/////////////////////////////////////////////////////////////////////////////
// Name:        sound.cpp
// Purpose:     wxSound class implementation: optional
// Author:      Ryan Norton, Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Ryan Norton, Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "sound.h"
#endif

#include "wx/object.h"
#include "wx/string.h"
#include "wx/log.h"
#include "wx/file.h"
#include "wx/sound.h"
#include "wx/timer.h"

#if wxUSE_SOUND

// Carbon QT Implementation Details -
//
// Memory:
// 1) OpenDefaultComponent(MovieImportType, kQTFileTypeWave);
// 2) NewMovie(0);
// 3) MovieImportDataRef() //Pass Memory Location to this
// 4) PlayMovie();
// 5) IsMovieDone(), MoviesTask() //2nd param is minimum wait time to allocate to quicktime
//
// File:
// 1) Obtain FSSpec
// 2) Call OpenMovieFile
// 3) Call NewMovieFromFile
// 4) Call CloseMovieFile
// 4) PlayMovie();
// 5) IsMovieDone(), MoviesTask() //2nd param is minimum wait time to allocate to quicktime
//

#ifdef __WXMAC__
#include "wx/mac/private.h"
#include <Movies.h>
#include <Gestalt.h>
#endif

#if defined __WXMAC__ && defined __DARWIN__/*TARGET_CARBON*/
#ifdef __APPLE_CC__
#include <Carbon/Carbon.h>
#else
#include <Carbon.h>
#endif
#else
#include <Sound.h>
#endif

//quicktime media layer only required for mac emulation on pc
#ifndef __WXMAC__
#include <qtml.h>
#endif

#include <QuickTimeComponents.h>

//Time inbetween timer calls
#define MOVIE_DELAY 100

// ------------------------------------------------------------------
//          wxQTTimer - Handle Asyncronous Playing
// ------------------------------------------------------------------
class wxQTTimer : public wxTimer
{
public:
    wxQTTimer(Movie movie, bool bLoop) :
        m_movie(movie), m_bLoop(bLoop)
    {
    }

    ~wxQTTimer()
    {
        Shutdown();
    }

    void Shutdown()
    {
        StopMovie(m_movie);
        DisposeMovie(m_movie);
        m_movie = NULL ;
        Stop();
    }

    void Notify()
    {
        if(IsMovieDone(m_movie))
        {
            if (!m_bLoop)
                Shutdown();
            else
            {
                StopMovie(m_movie);
                GoToBeginningOfMovie(m_movie);            
                StartMovie(m_movie);
        }
        }
        else
            MoviesTask(m_movie, MOVIE_DELAY); //Give QT time to play movie
    }


    Movie& GetMovie() {return m_movie;}
protected:
    Movie m_movie;
    bool m_bLoop;
};


class wxSMTimer : public wxTimer
{
public:
    wxSMTimer(void* hSnd, void* pSndChannel, const bool& bLoop)
	: m_hSnd(hSnd), m_pSndChannel(pSndChannel), m_bLoop(bLoop)
     {
     }

    ~wxSMTimer()
        {
	    Shutdown();
        }

    void Notify()
        {
    	SCStatus stat;

        if (SndChannelStatus((SndChannelPtr)m_pSndChannel, sizeof(SCStatus), &stat) != 0)
    	    Shutdown();
	
	    //if the sound isn't playing anymore, see if it's looped,
	    //and if so play it again, otherwise close things up
	    if (stat.scChannelBusy == FALSE)
	    {
	        if (m_bLoop)
	        {
		        if(SndPlay((SndChannelPtr)m_pSndChannel, (SndListHandle) m_hSnd, true) != noErr)
		            Shutdown();
	        }
	        else
    		    Shutdown();
        }
    }

    void Shutdown()
    {
        SndDisposeChannel((SndChannelPtr)m_pSndChannel, TRUE);
    	Stop();
    }
 
protected:
    void* m_hSnd;
    void* m_pSndChannel;
    bool m_bLoop;
};

// ------------------------------------------------------------------
//          wxSound
// ------------------------------------------------------------------

//Determines whether version 4 of QT is installed
Boolean wxIsQuickTime4Installed (void)
{
#ifdef __WXMAC__
    short error;
    long result;

    error = Gestalt (gestaltQuickTime, &result);
    return (error == noErr) && (result >= 4);  //result >= 4 correct?
#else
    return true;
#endif
}

inline bool wxInitQT ()
{
    if (wxIsQuickTime4Installed())
    {
        #ifndef __WXMAC__
        int nError;
        //-2093 no dll
            if ((nError = InitializeQTML(0)) != noErr)
                wxLogSysError(wxString::Format("Couldn't Initialize Quicktime-%i", nError));
        #endif 
        EnterMovies();
        return true;
    }
    else
        {
        wxLogSysError("Quicktime is not installed, or Your Version of Quicktime is <= 4.");
            return false;
        }
}

wxSound::wxSound()
: m_hSnd(NULL), m_waveLength(0), m_pTimer(NULL), m_type(wxSound_NONE)
{
}

wxSound::wxSound(const wxString& sFileName, bool isResource)
: m_hSnd(NULL), m_waveLength(0), m_pTimer(NULL), m_type(wxSound_NONE)
{
    Create(sFileName, isResource);
}

wxSound::wxSound(int size, const wxByte* data)
: m_hSnd((char*)data), m_waveLength(size), m_pTimer(NULL), m_type(wxSound_MEMORY)
{
    if (!wxInitQT())
        m_type = wxSound_NONE;
}

wxSound::~wxSound()
{
    FreeData();
}

bool wxSound::Create(const wxString& fileName, bool isResource)
{
    if(!wxInitQT())
        return false;

    if (isResource)
    {
#ifdef __WXMAC__
        m_type = wxSound_RESOURCE;

        Str255 lpSnd ;

        wxMacStringToPascal( fileName , lpSnd ) ;

		m_sndname = lpSnd;
        m_hSnd = (char*) GetNamedResource('snd ', (const unsigned char *) lpSnd);
#else
        return false;
#endif
        }
        else
        {
        m_type = wxSound_FILE;
        m_sndname = fileName;
    }

    return true;
}

bool wxSound::DoPlay(unsigned flags) const
{
    wxASSERT(m_pTimer == NULL || !((wxTimer*)m_pTimer)->IsRunning() );

    Movie movie;

    switch(m_type)
    {
    case wxSound_MEMORY:
        {
            Handle myHandle, dataRef = nil;
            MovieImportComponent miComponent; 
            Track targetTrack = nil;
            TimeValue addedDuration = 0;
            long outFlags = 0;
            OSErr err;
            ComponentResult result;

            myHandle = NewHandleClear((Size)m_waveLength);
            
            BlockMove(m_hSnd, *myHandle, m_waveLength);

            err = PtrToHand(&myHandle, &dataRef, sizeof(Handle));

            if (memcmp(&m_hSnd[8], "WAVE", 4) == 0)
                miComponent = OpenDefaultComponent(MovieImportType, kQTFileTypeWave);
            else if (memcmp(&m_hSnd[8], "AIFF", 4) == 0)
                miComponent = OpenDefaultComponent(MovieImportType, kQTFileTypeAIFF);
            else if (memcmp(&m_hSnd[8], "AIFC", 4) == 0)
                miComponent = OpenDefaultComponent(MovieImportType, kQTFileTypeAIFC);
            else
    {
                wxLogSysError("wxSound - Location in memory does not contain valid data");
                return false;
            }

            movie = NewMovie(0);

            result = MovieImportDataRef(miComponent,                dataRef,
                                        HandleDataHandlerSubType,   movie,
                                        nil,                        &targetTrack,
                                        nil,                        &addedDuration,
                                        movieImportCreateTrack,     &outFlags);

            if (result != noErr)
            {
                wxLogSysError(wxString::Format(wxT("Couldn't import movie data\nError:%i"), (int)result));
    }

            SetMovieVolume(movie, kFullVolume);
            GoToBeginningOfMovie(movie);
        }
        break;
    case wxSound_RESOURCE:
        {
            SoundComponentData data;
            unsigned long numframes, offset;

            ParseSndHeader((SndListHandle)m_hSnd, &data, &numframes, &offset);
            //m_waveLength = numFrames * data.numChannels;

            SndChannelPtr pSndChannel;
            SndNewChannel(&pSndChannel, sampledSynth,
			            initNoInterp + 
                        (data.numChannels == 1 ? initMono : initStereo), NULL);

            if(SndPlay(pSndChannel, (SndListHandle) m_hSnd, flags & wxSOUND_ASYNC ? 1 : 0) != noErr)
	            return false;

            if (flags & wxSOUND_ASYNC)
    {
                ((wxSMTimer*&)m_pTimer) = new wxSMTimer(pSndChannel, m_hSnd, flags & wxSOUND_LOOP ? 1 : 0);
 
	            ((wxTimer*)m_pTimer)->Start(MOVIE_DELAY, wxTIMER_CONTINUOUS); 
    }
    else
                SndDisposeChannel(pSndChannel, TRUE);

            return true;
        }
        break;
    case wxSound_FILE:
        {
            short movieResFile;
            FSSpec sfFile;

#ifdef __WXMAC__
            wxMacFilename2FSSpec( m_sndname , &sfFile ) ;
#else
            int nError;
            if ((nError = NativePathNameToFSSpec ((char*) m_sndname.c_str(), &sfFile, 0)) != noErr)
    {
                wxLogSysError(wxString::Format(wxT("File:%s does not exist\nError:%i"), 
                                m_sndname.c_str(), nError));
                return false;
    }
#endif

            if (OpenMovieFile (&sfFile, &movieResFile, fsRdPerm) != noErr)
            {
                wxLogSysError(wxT("Quicktime couldn't open the file"));
                return false;
            }


            short movieResID = 0;
            Str255 movieName;
            OSErr err;

            err = NewMovieFromFile (
            &movie,
            movieResFile,
            &movieResID,
            movieName,
            newMovieActive,
            NULL); //wasChanged

            CloseMovieFile (movieResFile);

            if (err != noErr)
    {
                wxLogSysError(
                    wxString::Format(wxT("wxSound - Could not open file: %s\nError:%i"), m_sndname.c_str(), err )
                    );
                return false;
    }
        }
        break;
    default:
        return false;
    }//end switch(m_type)


    //Start the movie!
    StartMovie(movie);

    if (flags & wxSOUND_ASYNC)
    {
        //Start timer and play movie asyncronously
        ((wxQTTimer*&)m_pTimer) = new wxQTTimer(movie, flags & wxSOUND_LOOP ? 1 : 0);
        ((wxQTTimer*)m_pTimer)->Start(MOVIE_DELAY, wxTIMER_CONTINUOUS);
    }
    else
    {
        wxASSERT_MSG(!(flags & wxSOUND_LOOP), "Can't loop and play syncronously at the same time");

        //Play movie until it ends, then exit
        while (!IsMovieDone(movie))
            MoviesTask(movie, 0);

        DisposeMovie(movie);
    }

    return true;
}

void* wxSound::GetHandle() 
{
    return (void*) ((wxQTTimer*) m_pTimer)->GetMovie();
}

bool wxSound::FreeData()
{
    if (m_pTimer != NULL)
    {
        delete (wxQTTimer*) m_pTimer;
        m_pTimer = NULL;
    }

    //Note that ExitMovies() is not neccessary, but
    //the docs are fuzzy on whether or not TerminateQTML is
    ExitMovies();

#ifndef __WXMAC__
    TerminateQTML();
#endif
    return true;
}
#endif //wxUSE_SOUND

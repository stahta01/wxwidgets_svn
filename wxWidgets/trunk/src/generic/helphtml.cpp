/*-*- c++ -*-********************************************************
 * helphtml.cpp - base class for html help systems                  *
 *                                                                  *
 * (C) 1999 by Karsten Ball�der (Ballueder@usa.net)                 *
 *                                                                  *
 * $Id$
 *******************************************************************/
#ifdef __GNUG__
#   pragma implementation "helphtml.h"
#endif

#include   "wx/setup.h"
#include   "wx/helpbase.h"
#include   "wx/generic/helpext.h"
#include   "wx/string.h"
#include   "wx/utils.h"
#include   "wx/list.h"
#include   "wx/intl.h"
#include   <stdio.h>
#include   <ctype.h>
#include   <sys/stat.h>
#include   <unistd.h>



class wxExtHelpMapEntry : public wxObject
{
public:
   int      id;
   wxString url;
   wxString doc;
   wxExtHelpMapEntry(int iid, wxString const &iurl, wxString const &idoc)
      { id = iid; url = iurl; doc = idoc; }
};

IMPLEMENT_ABSTRACT_CLASS(wxHTMLHelpControllerBase, wxHelpControllerBase)
   
/**
   This class implements help via an external browser.
   It requires the name of a directory containing the documentation
   and a file mapping numerical Section numbers to relative URLS.
*/

wxHTMLHelpControllerBase::wxHTMLHelpControllerBase(void)
{
   m_MapList = (wxList*) NULL;
   m_NumOfEntries = 0;
}

void
wxHTMLHelpControllerBase::DeleteList(void)
{
   if(m_MapList)
   {
      wxNode *node = m_MapList->First();
      while (node)
      {
         delete (wxExtHelpMapEntry *)node->Data();
         delete node;
         node = m_MapList->First();
      }
      delete m_MapList;
      m_MapList = (wxList*) NULL;
   }
}

wxHTMLHelpControllerBase::~wxHTMLHelpControllerBase(void)
{
   DeleteList();
}

/** This must be called to tell the controller where to find the
    documentation.
    @param file - NOT a filename, but a directory name.
    @return true on success
*/
bool
wxHTMLHelpControllerBase::Initialize(const wxString& file)
{
   return LoadFile(file);
}


// ifile is the name of the base help directory
bool
wxHTMLHelpControllerBase::LoadFile(const wxString& ifile)
{
   wxString mapFile, file, url, doc;
   int id,i,len;
   char buffer[WXEXTHELP_BUFLEN];
   
   wxBusyCursor b; // display a busy cursor

   if(! ifile.IsEmpty())
   {
      file = ifile;
      if(! wxIsAbsolutePath(file))
      {
         char* f = wxGetWorkingDirectory();
         file = f;
         delete[] f; // wxGetWorkingDirectory returns new memory
         file << WXEXTHELP_SEPARATOR << ifile;
      }
      else
         file = ifile;

      // If a locale is set, look in file/localename, i.e.
      // If passed "/usr/local/myapp/help" and the current wxLocale is
      // set to be "de", then look in "/usr/local/myapp/help/de/"
      // first and fall back to "/usr/local/myapp/help" if that
      // doesn't exist.
      if(wxGetLocale() && !wxGetLocale()->GetName().IsEmpty())
      {
         wxString newfile;
         newfile << WXEXTHELP_SEPARATOR << wxGetLocale()->GetName();
         if(wxDirExists(newfile))
            file = newfile;
         else
         {
            newfile = WXEXTHELP_SEPARATOR;
            const char *cptr = wxGetLocale()->GetName().c_str();
            while(*cptr && *cptr != '_')
               newfile << *(cptr++);
            if(wxDirExists(newfile))
               file = newfile;
         }
      }
      
      if(! wxDirExists(file))
         return FALSE;
      
      mapFile << file << WXEXTHELP_SEPARATOR << WXEXTHELP_MAPFILE;
   }
   else // try to reload old file
      mapFile = m_MapFile;

   if(! wxFileExists(mapFile))
      return FALSE;

   DeleteList();
   m_MapList = new wxList;
   m_NumOfEntries = 0;
   
   FILE *input = fopen(mapFile.c_str(),"rt");
   if(! input)
      return FALSE;
   do
   {
      if(fgets(buffer,WXEXTHELP_BUFLEN,input) && *buffer != WXEXTHELP_COMMENTCHAR)
      {
         len = strlen(buffer);
         if(buffer[len-1] == '\n')
            buffer[len-1] = '\0'; // cut of trailing newline
         if(sscanf(buffer,"%d", &id) != 1)
            break; // error
         for(i=0; isdigit(buffer[i])||isspace(buffer[i])||buffer[i]=='-'; i++)
            ; // find begin of URL
         url = "";
         while(buffer[i] && ! isspace(buffer[i]) && buffer[i] !=
               WXEXTHELP_COMMENTCHAR)
            url << buffer[i++];
         while(buffer[i] && buffer[i] != WXEXTHELP_COMMENTCHAR)
            i++;
         doc = "";
         if(buffer[i])
            doc = (buffer + i + 1); // skip the comment character
         m_MapList->Append(new wxExtHelpMapEntry(id,url,doc));
         m_NumOfEntries++;
      }
   }while(! feof(input));
   fclose(input);
   
   m_MapFile = file; // now it's valid
   return TRUE;
}


bool
wxHTMLHelpControllerBase::DisplayContents(void)
{
   if(! m_NumOfEntries)
      return FALSE;
   wxBusyCursor b; // display a busy cursor
   return KeywordSearch("");
}
      
bool
wxHTMLHelpControllerBase::DisplaySection(int sectionNo)
{
   if(! m_NumOfEntries)
      return FALSE;

   wxBusyCursor b; // display a busy cursor
   wxNode *node = m_MapList->First();
   wxExtHelpMapEntry *entry;
   while(node)
   {
      entry = (wxExtHelpMapEntry *)node->Data();
      if(entry->id == sectionNo)
         return DisplayHelp(entry->url);
      node = node->Next();
   }
   return FALSE;
}

bool
wxHTMLHelpControllerBase::DisplayBlock(long blockNo)
{
   return DisplaySection((int)blockNo);
}

bool
wxHTMLHelpControllerBase::KeywordSearch(const wxString& k)
{
   if(! m_NumOfEntries)
      return FALSE;

   wxBusyCursor b; // display a busy cursor
   wxString     *choices = new wxString[m_NumOfEntries];
   wxString     *urls = new wxString[m_NumOfEntries];
   wxString compA, compB;
   
   int          idx = 0, j;
   bool         rc;
   bool         showAll = k.IsEmpty();
   wxNode       *node = m_MapList->First();
   wxExtHelpMapEntry *entry;
   
   compA = k; compA.LowerCase(); // we compare case insensitive
   while(node)
   {
      entry = (wxExtHelpMapEntry *)node->Data();
      compB = entry->doc; compB.LowerCase();
      if((showAll || compB.Contains(k)) && ! compB.IsEmpty()) 
      {
         urls[idx] = entry->url;
         // doesn't work:
         // choices[idx] = (**i).doc.Contains((**i).doc.Before(WXEXTHELP_COMMENTCHAR));
         //if(choices[idx].IsEmpty()) // didn't contain the ';'
         //   choices[idx] = (**i).doc;
         choices[idx] = "";
         for(j=0;entry->doc.c_str()[j]
                && entry->doc.c_str()[j] != WXEXTHELP_COMMENTCHAR; j++)
            choices[idx] << entry->doc.c_str()[j];
         idx++;
      }
      node = node->Next();
   }

   if(idx == 1)
      rc = DisplayHelp(urls[0]);
   else if(idx == 0)
   {
      wxMessageBox(_("No entries found."));
      rc = FALSE;
   }
   else
   {
      idx = wxGetSingleChoiceIndex(showAll ? _("Help Index") : _("Relevant entries:"),
                                   showAll ? _("Help Index") : _("Entries found"),
                                   idx,choices);
      if(idx != -1)
         rc = DisplayHelp(urls[idx]);
      else
         rc = FALSE;
   }
   delete[] urls;
   delete[] choices;
   
   return rc;
}


bool
wxHTMLHelpControllerBase::Quit(void)
{
   return TRUE;
}

void
wxHTMLHelpControllerBase::OnQuit(void)
{
}


/*-*- c++ -*-********************************************************
 * wxlparser.h : parsers,  import/export for wxLayoutList           *
 *                                                                  *
 * (C) 1998,1999 by Karsten Ball�der (Ballueder@usa.net)            *
 *                                                                  *
 * $Id$
 *******************************************************************/

#ifdef __GNUG__
#   pragma implementation "wxlparser.h"
#endif

//#include "Mpch.h"
#ifdef M_PREFIX
#   include "gui/wxllist.h"
#   include "gui/wxlparser.h"
#else
#   include "wxllist.h"
#   include "wxlparser.h"
#endif

#define   BASE_SIZE 12

inline static bool IsEndOfLine(const char *p, int mode)
{
   // in addition to Unix EOL convention we also (but not instead) understand
   // the DOS one under Windows
   return
      ((mode & WXLO_EXPORT_WITH_MASK) == WXLO_EXPORT_WITH_CRLF) ?
      ((*p == '\r') && (*(p + 1) == '\n')) 
      :
      (((*p == '\r') && (*(p + 1) == '\n'))||(*p == '\n'));
}

void wxLayoutImportText(wxLayoutList &list, wxString const &str, int withflag)
{
   char * cptr = (char *)str.c_str(); // string gets changed only temporarily
   const char * begin = cptr;
   char  backup;
   
   for(;;)
   {
      begin = cptr;
      while( *cptr && !IsEndOfLine(cptr, withflag) )
         cptr++;
      backup = *cptr;
      *cptr = '\0';
      list.Insert(begin);
      *cptr = backup;

      // check if it's the end of this line
      if ( IsEndOfLine(cptr, withflag) )
      {
         // if it was "\r\n", skip the following '\n'
         if ( *cptr == '\r' )
            cptr++;
         list.LineBreak();
      }
      else if(backup == '\0') // reached end of string
         break;
      cptr++;
   }
}

static
wxString wxLayoutExportCmdAsHTML(wxLayoutObjectCmd const & cmd,
                                 wxLayoutStyleInfo *styleInfo)
{
   static char buffer[20];
   wxString html;
   
   wxLayoutStyleInfo si;
   cmd.GetStyle(&si);

   int size, sizecount;
   
   html += "<font ";

   html +="color=";
   sprintf(buffer,"\"#%02X%02X%02X\"", si.fg_red,si.fg_green,si.fg_blue);
   html += buffer;


   html += " bgcolor=";
   sprintf(buffer,"\"#%02X%02X%02X\"", si.bg_red,si.bg_green,si.bg_blue);
   html += buffer;

   switch(si.family)
   {
   case wxSWISS:
   case wxMODERN:
      html += " face=\"Arial,Helvetica\""; break;
   case wxROMAN:
      html += " face=\"Times New Roman, Times\""; break;
   case wxTELETYPE:
      html += " face=\"Courier New, Courier\""; break;
   default:
      ;
   }

   size = BASE_SIZE; sizecount = 0;
   while(size < si.size && sizecount < 5)
   {
      sizecount ++;
      size = (size*12)/10;
   }
   while(size > si.size && sizecount > -5)
   {
      sizecount --;
      size = (size*10)/12;
   }
   html += "size=";
   sprintf(buffer,"%+1d", sizecount);
   html += buffer;

   html +=">";

   if(styleInfo != NULL)
      html ="</font>"+html; // terminate any previous font command

   if((si.weight == wxBOLD) && ( (!styleInfo) || (styleInfo->weight != wxBOLD)))
      html += "<b>";
   else
      if(si.weight != wxBOLD && ( styleInfo && (styleInfo->weight == wxBOLD)))
         html += "</b>";

   if(si.style == wxSLANT)
      si.style = wxITALIC; // the same for html
   
   if((si.style == wxITALIC) && ( (!styleInfo) || (styleInfo->style != wxITALIC)))
      html += "<i>";
   else
      if(si.style != wxITALIC && ( styleInfo && (styleInfo->style == wxITALIC)))
         html += "</i>";

   if(si.underline && ( (!styleInfo) || ! styleInfo->underline))
      html += "<u>";
   else if(si.underline == false && ( styleInfo && styleInfo->underline))
      html += "</u>";

   
   *styleInfo = si; // update last style info
   
   return html;
}



#define   WXLO_IS_TEXT(type) \
( type == WXLO_TYPE_TEXT \
  || (type == WXLO_TYPE_CMD \
      && (mode & WXLO_EXPORT_AS_MASK) == WXLO_EXPORT_AS_HTML))


  
wxLayoutExportObject *wxLayoutExport(wxLayoutExportStatus *status,
                                     int mode)
{
   wxASSERT(status);
   wxLayoutExportObject * export;

   if(status->m_iterator == NULLIT) // end of line
   {
      if(!status->m_line || status->m_line->GetNextLine() == NULL) // reached end of list
         return NULL;
      else
      {
         status->m_line = status->m_line->GetNextLine();
         status->m_iterator = status->m_line->GetFirstObject();
         export = new wxLayoutExportObject();;
         export->type = ((mode & WXLO_EXPORT_AS_MASK) == WXLO_EXPORT_AS_HTML)
            ?  WXLO_EXPORT_HTML : WXLO_EXPORT_TEXT;
         if((mode & WXLO_EXPORT_WITH_CRLF) == WXLO_EXPORT_WITH_CRLF)
            export->content.text = new wxString("\r\n");
         else
            export->content.text = new wxString("\n");
         return export;
      }
   }
   
   export = new wxLayoutExportObject();
   wxLayoutObjectType   type = (** status->m_iterator).GetType();
   if( (mode & WXLO_EXPORT_AS_MASK) == WXLO_EXPORT_AS_OBJECTS || ! WXLO_IS_TEXT(type)) // simple case
   {
      export->type = WXLO_EXPORT_OBJECT;
      export->content.object = *status->m_iterator;
      status->m_iterator++;
      return export;
   }

   // else: must be text
   wxString *str = new wxString();
   // text must be concatenated
   do
   {
      switch(type)
      {
      case WXLO_TYPE_TEXT:
         *str += ((wxLayoutObjectText *)*status->m_iterator)->GetText();
         break;
      case WXLO_TYPE_CMD:
         wxASSERT_MSG( (mode&WXLO_EXPORT_AS_MASK) == WXLO_EXPORT_AS_HTML,
                       "reached cmd object in text mode" );
         
         *str += wxLayoutExportCmdAsHTML(*(wxLayoutObjectCmd const
                                           *)*status->m_iterator, & status->m_si);
         break;
      default:  // ignore icons
         ;
      }
      status->m_iterator++;
      if(status->m_iterator == NULLIT) // end of line!
      {
         if((mode & WXLO_EXPORT_AS_MASK) == WXLO_EXPORT_AS_HTML)
            *str += "<br>";
         if((mode & WXLO_EXPORT_WITH_CRLF) == WXLO_EXPORT_WITH_CRLF)
            *str += "\r\n";
         else
            *str += '\n';
         status->m_line = status->m_line->GetNextLine();
         if(status->m_line)
            status->m_iterator = status->m_line->GetFirstObject();
         else
            status->m_iterator = NULLIT;
      }
      if(status->m_iterator != NULLIT)
         type = (** status->m_iterator).GetType();
      else
         break;
   }
   while(WXLO_IS_TEXT(type));

   export->type = ((mode & WXLO_EXPORT_AS_MASK) == WXLO_EXPORT_AS_HTML)
      ?  WXLO_EXPORT_HTML : WXLO_EXPORT_TEXT;
   export->content.text = str;
   return export;
}


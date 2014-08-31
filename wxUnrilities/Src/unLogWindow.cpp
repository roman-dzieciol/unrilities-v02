// ============================================================================
//  Unrilities :: Cross-platform toolkit for Unreal Engine packages
// ============================================================================
//  Copyright (C) 2005  Roman Dzieciol <neai at o2.pl>
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// ============================================================================
//	unLogWindow.cpp
// ============================================================================
#include "prec.h"
#include "unLogWindow.h"


BEGIN_EVENT_TABLE	( unLogFrame, wxFrame )
	EVT_CLOSE		( unLogFrame::OnCloseWindow )
END_EVENT_TABLE()



// ============================================================================
// unLogFrame
// ============================================================================

unLogFrame::unLogFrame( wxWindow* parent, unLogWindow* log, const wxChar* caption )
:	Log(log)
,	wxFrame( parent, wxID_ANY, caption, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT )
{
	Text = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxHSCROLL | wxTE_READONLY | wxTE_RICH );
	Text->SetBackgroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR ) );
}

void unLogFrame::DoClose()
{
	// instead of closing just hide the window to be able to Show() it later
	Show(false);
}

void unLogFrame::OnCloseWindow( wxCloseEvent& WXUNUSED(event) )
{
	DoClose();
}

unLogFrame::~unLogFrame()
{
	SaveLog();
}

void unLogFrame::SaveLog()
{
	wxString filename = wxT("Unrilities.log");
	wxFile file( filename, wxFile::write );
	wxString text = Text->GetValue();
	text.Replace( wxT("\n"), wxTextFile::GetEOL(), true );
	file.Write( text );
	file.Close();
}


// ============================================================================
// unLogWindow
// ============================================================================
unLogWindow::unLogWindow( wxWindow* parent, const wxChar* caption, bool bpassmsg )
{
	PassMessages( bpassmsg );
	LogFrame = new unLogFrame( parent, this, caption );

	wxDateTime today = wxDateTime::Today();
	wxLogMessage( wxT("Log file open: %s"), today.FormatISODate().wx_str() );
}

unLogWindow::~unLogWindow()
{
	// may be NULL if log frame already auto destroyed itself
	//delete LogFrame;
}

void unLogWindow::Show( bool bShow )
{
	LogFrame->Show( bShow );
}

void unLogWindow::DoLogTextAtLevel (wxLogLevel level, const wxChar *str)
{
	// first let the previous logger show it
	wxLogPassThrough::DoLogTextAtLevel(level, str);
	
	if( LogFrame ) 
	{
		switch( level ) 
		{
			case wxLOG_Status:
				// by default, these messages are ignored by wxLog, so process
				// them ourselves
				if( !wxIsEmpty(str) )
				{
					wxString str;
					str << _("Status: ") << str;
					DoLogText(str.wx_str());
				}
				break;

				// don't put trace messages in the text window for 2 reasons:
				// 1) there are too many of them
				// 2) they may provoke other trace messages thus sending a program
				//    into an infinite loop
			case wxLOG_Trace:
				break;

			default:
				// and this will format it nicely and call our DoLogString()
				wxLog::DoLogTextAtLevel(level, str);
		}
	}
}

void unLogWindow::DoLogText( const wxChar* str )
{
	// put the text into our window
	wxTextCtrl* text = LogFrame->TextCtrl();

#ifdef __WXMSW__
	// remove selection (WriteText is in fact ReplaceSelection)
	wxTextPos len = text->GetLastPosition();
	text->SetSelection(len, len);
#endif

	wxString msg;
	TimeStamp(&msg);
	msg << str << wxT('\n');
	text->AppendText(msg);
}

wxFrame *unLogWindow::GetFrame() const
{
	return LogFrame;
}




// ============================================================================
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
//	wxUnrilities.cpp
// ============================================================================
#include "prec.h"
#include "wxUnrilities.h"

#include "pkgSerializer.h"
#include "unStatusBar.h"
#include "unLogWindow.h"
#include "unNotebook.h"
#include "unFlagManager.h"


// ----------------------------------------------------------------------------
// images
// ----------------------------------------------------------------------------
#include "open.xpm"
#include "log.xpm"
#include "help.xpm"


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	Menu_FileOpen = wxID_OPEN,
	Menu_FileExit = wxID_EXIT,
	Menu_HelpAbout = wxID_ABOUT,

	Toolbar_Open = wxID_OPEN,
	Toolbar_About = wxID_ABOUT,

	Menu_ViewLog = wxID_HIGHEST + 1,

	Toolbar_Log = Menu_ViewLog,

	ID_TOOLBAR,
	ID_NAVTREE,
	ID_GENLIST,
	ID_NOTEBOOK
};


const wxString	wxUnrilitiesFrame::Title = wxT("Unrilities");
const wxString	wxUnrilitiesFrame::Build = wxT("v0.3");


// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE	( wxUnrilitiesFrame, wxFrame )

	EVT_MENU		( wxID_OPEN,	wxUnrilitiesFrame::OnOpen )
	EVT_MENU		( wxID_EXIT,	wxUnrilitiesFrame::OnExit )
	EVT_MENU		( wxID_ABOUT,	wxUnrilitiesFrame::OnAbout )

	EVT_MENU		( Menu_ViewLog,	wxUnrilitiesFrame::OnViewLog )

	EVT_CLOSE		( wxUnrilitiesFrame::OnClose )

END_EVENT_TABLE()



// ============================================================================
// wxUnrilitiesApp
// ============================================================================
bool wxUnrilitiesApp::OnInit()
{
	// calc window position
	wxRect win = CalcFrame();

	// create frame
	wxUnrilitiesFrame* frame = new wxUnrilitiesFrame( NULL, wxID_ANY, wxUnrilitiesFrame::Title, win.GetPosition(), win.GetSize() );
	frame->SetIcon(wxICON(ICO_App));
	frame->ShowBoth(true);
	frame->SetFocus();

	return true;
}

// ----------------------------------------------------------------------------
// CalcFrame
// ----------------------------------------------------------------------------
wxRect wxUnrilitiesApp::CalcFrame()
{
	wxSize screen( wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) );
	wxRect win;

	if( screen.IsFullySpecified() )
	{
		win.SetSize( wxSize(	static_cast<int>(screen.GetWidth()*0.95f), 
								static_cast<int>(screen.GetHeight()*0.66f) ));

		win.SetPosition( wxPoint(	static_cast<int>((screen.GetWidth()-win.GetWidth())*0.5f), 
									static_cast<int>((screen.GetHeight()-win.GetHeight())*0.5f) - 
									static_cast<int>(screen.GetHeight()*0.12f) ) );
	}
	else
	{
		win.SetSize( wxDefaultSize );
		win.SetPosition( wxDefaultPosition );
	}
	return win;
}

IMPLEMENT_APP(wxUnrilitiesApp)


// ============================================================================
// wxUnrilitiesFrame
// ============================================================================
wxUnrilitiesFrame::wxUnrilitiesFrame
(	wxWindow*		parent
,	wxWindowID		id
,	const wxString&	title
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
)	
:	Serializer(NULL)
,	wxFrame( parent, id, title, pos, size, style, name )
{
	SW_DEF( sw );

	// Flag manager
	GFlag.LoadFlags();

	// Log
	LogWindow = new wxLogWindow( this, wxT("Unrilities Log") );
	LogWindow->GetFrame()->SetSize( GetPosition().x, GetPosition().y+GetSize().GetHeight(), GetSize().GetWidth(), static_cast<int>(GetSize().GetHeight()*0.3f) );
	LogWindow->GetFrame()->SetIcon( wxICON(ICO_Log) );
	//LogWindow->GetFrame()->SetFont( GetFont() );

	// menu file
	MenuFile = new wxMenu;
	MenuFile->Append(Menu_FileOpen, wxT("&Open"), wxT("Open file"));
	MenuFile->AppendSeparator();
	MenuFile->Append(Menu_FileExit, wxT("E&xit"), wxT("Exit"));

	// menu view
	MenuView = new wxMenu;
	MenuView->Append(Menu_ViewLog, wxT("&Log"), wxT("Toggle Log Window"));

	// menu help
	MenuHelp = new wxMenu;
	MenuHelp->Append(Menu_HelpAbout, wxT("&About"), wxT("About"));

	// menu bar
	Menu = new wxMenuBar;
	Menu->Append(MenuFile, wxT("&File"));
	Menu->Append(MenuView, wxT("&View"));
	Menu->Append(MenuHelp, wxT("&Help"));
	SetMenuBar(Menu);

	// StatusBar
	StatusBar = static_cast<unStatusBar*>(CreateStatusBar());

	// ToolBar
	ToolBar = CreateToolBar( wxTB_NODIVIDER | wxTB_VERTICAL | wxTB_FLAT, wxID_ANY );
	//ToolBar->AddTool(Toolbar_Open, ("Open file"), wxNullBitmap);
	//ToolBar->AddTool(Toolbar_Log, ("Toggle Log"), wxNullBitmap);
	//ToolBar->AddTool(Toolbar_About, ("About Unrilities"), wxNullBitmap);
	ToolBar->Realize();

	// Notebook
	Notebook = new unNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );

	SetOwnBackgroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_MENU ) );

	SW_LOG( sw, "Init: %s" );
}

// ----------------------------------------------------------------------------
// ShowFocused
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::ShowBoth( bool bshow )
{
	Show( bshow );
	LogWindow->Show( bshow );
}

// ----------------------------------------------------------------------------
// OnOpen
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::OnOpen( wxCommandEvent& WXUNUSED(event) )
{
	wxFileDialog fd( this, wxT("Open file"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_OPEN );
	if( fd.ShowModal() == wxID_OK )
	{ 
		try
		{
			DoFileOpen( fd );
		}
		catch( exception& e )
		{
			wxLogError(wxT("%s -- %s"), fd.GetPath().wx_str(), e.what()); 
			LogWindow->Show(true);
			DoFileClose();
		}
	}
}

// ----------------------------------------------------------------------------
// OnExit
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::OnExit( wxCommandEvent& WXUNUSED(event) )
{
	ShowBoth(false);
	Close(true);
}

// ----------------------------------------------------------------------------
// OnViewLog
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::OnViewLog( wxCommandEvent& WXUNUSED(event) )
{
	if( LogWindow->GetFrame()->IsShown() )
			LogWindow->Show(false);
	else	LogWindow->Show(true);
}

// ----------------------------------------------------------------------------
// OnAbout
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
	wxString about;
	about << Title << wxT(" ") << Build << wxT("\nCross-platform toolkit for Unreal Engine package files.\n\nCopyright © 2005 Roman Dzieciol - neai@o2.pl");
	wxMessageBox( about, wxT("About Unrilities"), wxOK | wxICON_INFORMATION, this );
}

// ----------------------------------------------------------------------------
// OnCreateStatusBar
// ----------------------------------------------------------------------------
wxStatusBar* wxUnrilitiesFrame::OnCreateStatusBar( int number, long style, wxWindowID id, const wxString& name )
{
	wxStatusBar* s = new unStatusBar( this, id, style, name );
	s->SetFieldsCount(number);
	return s;
}

// ----------------------------------------------------------------------------
// OnClose
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::OnClose( wxCloseEvent& event )      
{
	ShowBoth(false);

	// close old file if open
	if( Serializer != NULL )
		DoFileClose();

	delete wxLog::SetActiveTarget(NULL);
	event.Skip();
}


// ----------------------------------------------------------------------------
// DoFileOpen
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::DoFileOpen( wxFileDialog& dialog )
{
	SW_DEF( sw );

	// close old file if open
	if( Serializer != NULL )
		DoFileClose();

	// open new file
	Serializer = new pkgSerializer();
	Serializer->OpenFile( dialog );

	// open file
	Notebook->OpenFile( Serializer );

	// Update frame
	SetTitle( Serializer->GetFileName() );
	SetStatusText( Serializer->GetFilePath() );

	SW_LOGV( sw, wxT("Open: %s %s"), dialog.GetPath().wx_str() );
}


// ----------------------------------------------------------------------------
// DoFileClose
// ----------------------------------------------------------------------------
void wxUnrilitiesFrame::DoFileClose()
{
	SW_DEF( sw );

	// Update Frame
	SetStatusText( wxT("") );
	SetTitle( wxUnrilitiesFrame::Title );

	// Clear Notebook
	Notebook->CloseFile();

	// Close file
	delete Serializer;
	Serializer = NULL;

	SW_LOG( sw, "Close: %s" ); wxLogMessage(wxT(""));
}

// ============================================================================
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
//	unFilterList.cpp
// ============================================================================
#include "prec.h"
#include "unFilterList.h"


#include "unLogToStatus.h"
#include "unTable.h"
#include "unExpression.h"


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	Menu_CaseSensitive = wxID_HIGHEST + 1,
	Menu_BackReferences,
	Menu_SyntaxExtended,
	Menu_SyntaxAdvanced,
	Menu_SyntaxBasic
};





// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE			( unFilterListCtrl, wxPanel )
    EVT_TEXT_ENTER			( wxID_ANY, unFilterListCtrl::OnTextEnter )
    EVT_TEXT				( wxID_ANY, unFilterListCtrl::OnText )
    EVT_SIZE				( unFilterListCtrl::OnSize )
    EVT_CONTEXT_MENU		( unFilterListCtrl::OnContextMenu ) 
    EVT_MENU				( Menu_CaseSensitive,	unFilterListCtrl::OnMenuCaseSensitive )
    EVT_MENU				( Menu_BackReferences,	unFilterListCtrl::OnMenuBackReferences )
    EVT_MENU_RANGE			( Menu_SyntaxExtended,	Menu_SyntaxBasic, unFilterListCtrl::OnMenuSyntax )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE			    ( unFilterList, unVirtualList )
    EVT_PAINT				    ( unFilterList::OnPaint )
    EVT_SCROLLWIN			    ( unFilterList::OnScrollWin ) 
    EVT_LIST_COL_END_DRAG	    ( wxID_ANY, unFilterList::OnColEndDrag ) 
END_EVENT_TABLE()


bool unFilterListCtrl::reCaseSensitive = false;
bool unFilterListCtrl::reBackReferences = true;
int unFilterListCtrl::reSyntax = wxRE_EXTENDED;


// ============================================================================
// unFilterListCtrl
// ============================================================================

unFilterListCtrl::unFilterListCtrl
(	wxWindow*		parent
 ,	wxWindowID		id
 ,	const wxPoint&	pos
 ,	const wxSize&	size
 ,	long			style
 ,	const wxString&	name
 ) 
 :	wxPanel(parent,id,pos,size,style,name)
{
	List = new unFilterList(this, wxID_ANY);
	List->Finder = this;
	EditPanel = new wxPanel(this,wxID_ANY,wxDefaultPosition,wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxTAB_TRAVERSAL );
}

void unFilterListCtrl::UpdateBoxes()
{
	// get my size
	wxSize mysize = GetClientSize();

	// get leftmost listitem position
	wxPoint itempos;
	List->GetItemPosition(0,itempos);

	// update sizes list
	EditSizes.resize( List->GetColumnCount() );

	// get list border
	wxRect rb,rl;
	List->GetItemRect(0,rb,wxLIST_RECT_BOUNDS);
	List->GetItemRect(0,rl,wxLIST_RECT_LABEL);
	dword listborder = rl.GetX() - rb.GetX();

	// update sizes
	dword width, acc = 0, y = 0;
	for(dword i=0; i<EditSizes.size(); ++i)
	{
		width = List->GetColumnWidth(i);

		// stretch first box to left edge
		if( i == 0 )
		{
			width += listborder;
			acc -= listborder;
		}

		EditSizes[i].SetX(acc+itempos.x);
		EditSizes[i].SetY(y);
		EditSizes[i].SetWidth(width);
		EditSizes[i].SetHeight(-1);
		acc += width;
	}

	// create new editboxes if neccessary
	if( EditSizes.size() > EditBoxes.size() )
	{
		EditBoxes.resize( EditSizes.size(), NULL );

		for( dword i=0; i<EditBoxes.size(); ++i )
		{
			if( EditBoxes[i] == NULL )
			{
				wxTextCtrl* box = new wxTextCtrl( EditPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0  );

				wxListItem item;
				List->GetColumn(i,item);
				wxString text = wxT("\"");
				text.Append( List->Table->GetHeader() AT(i).GetCaption().wx_str() );
				text.Append( wxT("\" filter") );
				box->SetToolTip( text );

				//wxString text;
				//text.Printf("%d",i);
				//EditBoxes[i]->WriteText(text);
				EditBoxes[i] = box;
			}
		}
	}

	// stretch last box to right edge
	if( !EditSizes.empty() )
	{
		wxRect lastbox = EditSizes.back();
		int lastpos = lastbox.GetX() + lastbox.GetWidth();
		//wxLogMessage("%d %d",lastpos, mysize.GetWidth());
		if( lastpos < mysize.GetWidth() )
		{
			EditSizes.back().SetWidth( lastbox.GetWidth() + (mysize.GetWidth() - lastpos) );
		}
	}

	// update editbox coords
	for( dword i=0; i<EditBoxes.size(); ++i )
	{
		//wxLogMessage("%d %d",itempos.x,ColumnX[i]);
		EditBoxes[i]->SetSize(EditSizes[i].GetSize());
		EditBoxes[i]->Move(EditSizes[i].GetPosition());
	}


	// set list size
	wxRect listsize( mysize );
	wxRect panelsize( mysize );
	if( !EditBoxes.empty() )
	{
		wxRect boxsize( EditBoxes[0]->GetBestSize() );
		panelsize.SetHeight( boxsize.GetHeight() );
		listsize.SetY( panelsize.GetHeight() );
		listsize.SetHeight( mysize.GetHeight()-panelsize.GetHeight() );
	}
	List->SetSize(listsize.GetSize());
	List->Move(listsize.GetPosition());

	EditPanel->SetSize(panelsize.GetSize());
	EditPanel->Move(panelsize.GetPosition());

	// fix editbox flicker
	for( dword i=0; i<EditBoxes.size(); ++i )
	{
		EditBoxes[i]->ClearBackground();
		EditBoxes[i]->Refresh();
	}
}



void unFilterListCtrl::OnTextEnter( wxCommandEvent& WXUNUSED(event) )
{
	//wxLogMessage( "OnTextEnter %s", event.GetString().wx_str() );
	try
	{

		vector<wxString> strings;
		for( dword i=0; i<EditBoxes.size(); ++i )
		{
			wxTextCtrl* edit = EditBoxes[i];
			strings.push_back( edit->GetValue() );
			edit->DiscardEdits();		
		}

		List->FilterEx( strings, GetRegExFlags() );
		//event.Skip();
	}
	catch( exception& e )
	{ 
		wxLogError(wxT("%s"), e.what()); 
	}
}


void unFilterListCtrl::OnText( wxCommandEvent& WXUNUSED(event) )
{
	//wxLogMessage( "OnText %s", event.GetString().wx_str() );
	int errorcount = 0;
	int flags = GetRegExFlags();
	const unTableFilter& filter = List->Table->GetFilter();

	for( dword i=0; i<EditBoxes.size(); ++i )
	{
		wxTextCtrl* edit = EditBoxes[i];
		unExpression* ex = filter AT(i);


		//wxLogMessage( "IsModified %d", i );
		wxLog* log = new unLogToStatus;
		ex->Compile( edit->GetValue(), flags );
		delete log;

		if( ex->IsValid() )
		{
			edit->SetOwnForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT) );
		}
		else
		{
			errorcount++;
			edit->SetOwnForegroundColour(*wxRED);
		}
		edit->Refresh();
	}

	if( errorcount == 0 )
	{
		wxWindow* topwin = wxTheApp->GetTopWindow();
		if( topwin != NULL && topwin->IsKindOf(CLASSINFO(wxFrame)) ) 
		{
			wxFrame* topframe = static_cast<wxFrame *>(topwin);
			topframe->SetStatusText(wxT(""));
		}
	}

	//event.Skip();
}

void unFilterListCtrl::OnSize( wxSizeEvent& event )
{
	UpdateBoxes();
	event.Skip();
}

void unFilterListCtrl::OnContextMenu( wxContextMenuEvent& event )
{
	wxPoint pt = event.GetPosition();
	//wxLogMessage(wxT("OnContextMenu at screen coords (%i, %i)"), pt.x, pt.y);

	if( pt.x == -1 && pt.y == -1 ) 
	{
		pt = wxWindow::FindFocus()->GetPosition();
	}
	else 
	{
        pt = ScreenToClient(pt);
        if( pt.y > List->GetPosition().y )
        {
            event.Skip();
            return;
        }
	}


	wxMenu menu(wxT("RegExp Options"));
	menu.AppendRadioItem(Menu_SyntaxExtended, wxT("&Extended Syntax"));
	menu.AppendRadioItem(Menu_SyntaxAdvanced, wxT("&Advanced Syntax"));
	menu.AppendRadioItem(Menu_SyntaxBasic, wxT("&Basic Syntax"));
	menu.AppendSeparator();
	menu.AppendCheckItem(Menu_CaseSensitive, wxT("Ignore &Case"));
	menu.AppendCheckItem(Menu_BackReferences, wxT("Don't set Back &References"));

	menu.Check( Menu_SyntaxExtended + unFilterListCtrl::reSyntax, true );
	menu.Check( Menu_CaseSensitive, !unFilterListCtrl::reCaseSensitive );
	menu.Check( Menu_BackReferences, !unFilterListCtrl::reBackReferences );

	PopupMenu(&menu, pt);

}

void unFilterListCtrl::OnMenuCaseSensitive( wxCommandEvent& WXUNUSED(event) )
{
	unFilterListCtrl::reCaseSensitive = !unFilterListCtrl::reCaseSensitive;
}

void unFilterListCtrl::OnMenuBackReferences( wxCommandEvent& WXUNUSED(event) )
{
	unFilterListCtrl::reBackReferences = !unFilterListCtrl::reBackReferences;
}

void unFilterListCtrl::OnMenuSyntax( wxCommandEvent& event )
{
	unFilterListCtrl::reSyntax = event.GetId()-Menu_SyntaxExtended;
}


int unFilterListCtrl::GetRegExFlags()
{
	int flags = 0;
	flags |= unFilterListCtrl::reCaseSensitive ? 0 : wxRE_ICASE;
	flags |= unFilterListCtrl::reBackReferences ? 0 : wxRE_NOSUB;
	flags |= unFilterListCtrl::reSyntax;
	return flags;
}

// ============================================================================
// unFilterList
// ============================================================================


unFilterList::unFilterList
(	wxWindow*			parent
 ,	wxWindowID			id
 ,	const wxPoint&		pos
 ,	const wxSize&		size
 ,	long				style
 ,	const wxValidator&	validator
 ,	const wxString&		name
 ) 
 :	Finder(NULL)
 ,	unVirtualList(parent,id,pos,size,style,validator,name)
{
}

void unFilterList::OnPaint( wxPaintEvent& event )
{
	//ClearBackground();
	Finder->UpdateBoxes();
	event.Skip();
}

void unFilterList::OnColEndDrag( wxListEvent& event ) 
{
	Finder->UpdateBoxes();
	Refresh();
	event.Skip();
}

void unFilterList::OnScrollWin(wxScrollWinEvent& event)
{
	Finder->UpdateBoxes();
	event.Skip();
}

void unFilterList::FilterEx( vector<wxString>& strings, int flags )
{
	dword matches = Table->FilterEx( strings, flags );
	SetItemCount( static_cast<long>(matches) );
	Refresh();
}




// ============================================================================
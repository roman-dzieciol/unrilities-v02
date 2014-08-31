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
//	unVirtualList.cpp
// ============================================================================
#include "prec.h"
#include "unVirtualList.h"

#include "unTable.h"


BEGIN_EVENT_TABLE			( unVirtualList, wxListView )
	EVT_LIST_COL_CLICK		( wxID_ANY, unVirtualList::OnColClick )
END_EVENT_TABLE()


// ============================================================================
// unVirtualList
// ============================================================================

unVirtualList::unVirtualList
(	wxWindow*			parent
,	wxWindowID			id
,	const wxPoint&		pos
,	const wxSize&		size
,	long				style
,	const wxValidator&	validator
,	const wxString&		name
) 
:	wxListView(parent,id,pos,size,style,validator,name)
{
	Table = new unTable;
}

unVirtualList::~unVirtualList()
{
	delete Table;
}

wxString unVirtualList::OnGetItemText( long item, long column ) const
{
	return wxString::Format(wxT("%d %d"), item, column);
	return Table->GetText( item, column );
}

void unVirtualList::OnColClick( wxListEvent& event ) 
{
	//wxLogMessage("OnColClick"); 
	Table->Sort( event.GetColumn() );
	Refresh();
	event.Skip();
}


// ============================================================================
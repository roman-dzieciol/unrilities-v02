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
//	unPackageView.cpp
// ============================================================================
#include "prec.h"
#include "unPackageView.h"

#include "unFilterList.h"
#include "unTable.h"




unPackageView::unPackageView
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	unNotebookPage(parent,id,pos,size,style,name)
{
}

unNotebookPage* unPackageView::Initialize( pkgSerializer* serializer )
{
	Serializer = serializer;
	Filter = new unFilterListCtrl(this);

	wxBoxSizer* vs = new wxBoxSizer( wxVERTICAL );
	vs->Add( Filter, 1, wxEXPAND, 0 );	
	vs->SetSizeHints( this );
	SetSizerAndFit( vs ); 

	//Show(false);

	InitData( *GetTable() );
	InitList( *GetList(), *GetTable() );

	return this;
}


void unPackageView::InitData( unTable& )
{
}

void unPackageView::InitList( unVirtualList& vlist, unTable& vtable )
{
	const unTableHeader& h( vtable.GetHeader() );
	for( unTableHeader::const_iterator it=h.begin(); it!=h.end(); ++it )
	{
		vlist.InsertColumn( it-h.begin(), (*it).GetCaption(), (*it).GetAlign(unFMT_LIST), (*it).GetWidth() );
	}

	vlist.SetItemCount( static_cast<long>(vtable.GetHeight()) );
}

unVirtualList* unPackageView::GetList()
{
	return Filter->List;
}

unTable* unPackageView::GetTable()
{
	return GetList()->Table;
}

bool unPackageView::Show( bool show )
{
	return wxPanel::Show(show);
}

void unPackageView::OnPageChanged()
{
    GetList()->SetFocus();
}

// ============================================================================
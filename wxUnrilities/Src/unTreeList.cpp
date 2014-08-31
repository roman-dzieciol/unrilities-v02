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
//	unTreeList.cpp
// ============================================================================
#include "prec.h"
#include "unTreeList.h"

#include "unDataTree.h"
#include "unTable.h"

// TODO: Fix buggy wxTreeListCtrl HitTest()
// TODO: Fix buggy wxTreeListCtrl scrolling


BEGIN_EVENT_TABLE(unTreeList, wxTreeListCtrl)
	EVT_TREE_ITEM_EXPANDING	( wxID_ANY, unTreeList::OnItemExpanding )
END_EVENT_TABLE()


// ============================================================================
// unTreeList
// ============================================================================
unTreeList::unTreeList
(	wxWindow*			parent
,	wxWindowID			id
,	const wxPoint&		pos
,	const wxSize&		size
,	long				style
,	const wxValidator&	validator
,	const wxString&		name
) 
:	wxTreeListCtrl(parent,id,pos,size,style,validator,name)
{
	Table = new unTable;
}

unTreeList::~unTreeList()
{
	delete Table;
}


void unTreeList::SetDataTree( unDataTree* tree )
{
	tree->SetTreeID( wxTreeListCtrl::AddRoot( tree->GetCaption(), -1, -1, tree->GetTreeData() ) );	
	//AppendItem(
	//AddPlaceholder( tree );
}

unDataTree* unTreeList::GetDataTree( wxTreeItemData* item ) const
{
	return SafePtr( static_cast<unDataTree::unDataTreeData*>(SafePtr(item))->GetTree() );
}

unDataTree* unTreeList::GetDataTree( const wxTreeItemId& item ) const
{
	return GetDataTree(GetItemData(SafeTreeID(item)));
}


void unTreeList::AddPlaceholder( unDataTree* parent )
{
	if( parent->ShouldExpand() )
		SetItemHasChildren( parent->GetTreeID(), true );
}

void unTreeList::AddChildren( unDataTree* parent )
{
	//SW_DEF( sw );
	for( unDataTree::iterator it=parent->begin(); it!=parent->end(); ++it )
	{
		unDataTree* tree = (*it);
		tree->SetTreeID( AppendItem( parent->GetTreeID(), tree->GetCaption(), -1, -1, tree->GetTreeData() ) );
		AddPlaceholder( tree );
	}
	//SW_LOGF( sw );
}

void unTreeList::SortChildren( const wxTreeItemId& item )
{
	//SW_DEF( sw );
	wxTreeListCtrl::SortChildren( item );
	//SW_LOGF( sw );
}


void unTreeList::ExpandTree( unDataTree* tree )
{
	stack<unDataTree*> parents;
	unDataTree* t;
	unDataTree* collapse = NULL;

	for( t = tree->GetParent(); t != NULL; t = t->GetParent() ) 
	{
		parents.push(t);
		if( collapse == NULL && t->GetSerialID() != -1 )
			collapse = t;
	} 

	while( !parents.empty() )
	{
		t = parents.top();
		if( t->ShouldExpand() )
		{
			EnsureVisible( t->GetTreeID() );
			Expand( t->GetTreeID() );
		}
		parents.pop();
	}

	EnsureVisible( tree->GetTreeID() );
	SelectItem( tree->GetTreeID() );
}

void unTreeList::ExpandRoot()
{		
	wxTreeItemId root = GetRootItem();
	if( root.IsOk() && HasChildren(root) && !IsExpanded(root) )
	{
		Expand(root);
	}
}


int unTreeList::OnCompareItems( const wxTreeItemId& item1, const wxTreeItemId& item2 )
{
	return GetItemText(item1).compare(GetItemText(item2));
}

void unTreeList::OnItemExpanding( wxTreeEvent& event )
{
	unDataTree* tree = GetDataTree( event.GetItem() );

	if( tree->Expand() )
	{
		AddChildren( tree );	
		if( tree->ShouldSort() )				
			SortChildren( tree->GetTreeID() );				
	}
}

wxString unTreeList::OnGetItemText( wxTreeItemData* item, long column ) const
{
	return wxString::Format(wxT("%p %d"), item, column);
	dword index = GetDataTree(item)->GetSerialID();
	if		( index != ID_IGNORE )			return Table->GetText( index, column );	
	else if	( column == GetMainColumn() )	return GetDataTree(item)->GetCaption();		
	else									return wxEmptyString;	
}


// ============================================================================
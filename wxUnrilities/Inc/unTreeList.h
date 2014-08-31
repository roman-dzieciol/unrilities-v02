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
//  unTreeList.h
// ============================================================================
#ifndef UNTREELIST_H
#define UNTREELIST_H


#include "wx/treelistctrl.h"

class unDataTree;
class unTable;


// ============================================================================
// unTreeList
// ============================================================================
class unTreeList : public wxTreeListCtrl
{
public:
	friend class unPackageTree;

public:
	unTreeList
	(	wxWindow*			parent
	,	wxWindowID			id = wxID_ANY
	,	const wxPoint&		pos = wxDefaultPosition
	,	const wxSize&		size = wxDefaultSize
	,	long				style = wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT | wxTR_FULL_ROW_HIGHLIGHT | wxTR_NO_LINES | wxTR_VIRTUAL | wxTR_SINGLE
	,	const wxValidator&	validator = wxDefaultValidator
	,	const wxString&		name = wxT("unTreeList")
	);

	virtual ~unTreeList();

public:
	void SetDataTree( unDataTree* tree );
	unDataTree* GetDataTree( wxTreeItemData* item ) const;
	unDataTree* GetDataTree( const wxTreeItemId& item ) const;

	void AddPlaceholder( unDataTree* parent );
	void AddChildren( unDataTree* parent );
	void SortChildren( const wxTreeItemId& item );

	void ExpandTree( unDataTree* tree );
	void ExpandRoot();

protected:
	void OnItemExpanding( wxTreeEvent& event );
	virtual int OnCompareItems( const wxTreeItemId& item1, const wxTreeItemId& item2 );
	virtual wxString OnGetItemText( wxTreeItemData* item, long column ) const;

protected:
	unTable*	Table;

private:
	DECLARE_EVENT_TABLE()
};




#endif UNTREELIST_H
// ============================================================================
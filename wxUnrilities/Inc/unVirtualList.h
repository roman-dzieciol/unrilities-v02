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
//	unVirtualList.h
// ============================================================================
#ifndef UNVIRTUALLIST_H
#define UNVIRTUALLIST_H

class unTable;


// ============================================================================
// unVirtualList
// ============================================================================
class unVirtualList : public wxListView
{
public:
	friend class unPackageFinder;
	friend class unPackageView;

public:
	unVirtualList
	(	wxWindow*			parent
	,	wxWindowID			id = wxID_ANY
	,	const wxPoint&		pos = wxDefaultPosition
	,	const wxSize&		size = wxDefaultSize
	,	long				style = wxLC_REPORT | wxLC_VIRTUAL
	,	const wxValidator&	validator = wxDefaultValidator
	,	const wxString&		name = wxT("unVirtualList")
	);

	virtual ~unVirtualList();

protected:
	void OnColClick( wxListEvent& event );
	virtual wxString OnGetItemText( long item, long column ) const;

protected:
	unTable*	Table;

private:
	DECLARE_EVENT_TABLE()
};


#endif UNVIRTUALLIST_H
// ============================================================================
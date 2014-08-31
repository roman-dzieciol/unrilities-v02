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
//	unFilterList.h
// ============================================================================
#ifndef UNFILTERLIST_H
#define UNFILTERLIST_H



#include "unVirtualList.h"

class unFilterListCtrl;



// ============================================================================
// unFilterList
// ============================================================================
class unFilterList : public unVirtualList
{
public:
	friend class unFilterListCtrl;

public:
	unFilterList
		(	wxWindow*			parent
		,	wxWindowID			id = wxID_ANY
		,	const wxPoint&		pos = wxDefaultPosition
		,	const wxSize&		size = wxDefaultSize
		,	long				style = wxLC_REPORT | wxLC_VIRTUAL | wxCLIP_CHILDREN
		,	const wxValidator&	validator = wxDefaultValidator
		,	const wxString&		name = wxT("unFilterList")
		);

	void OnPaint( wxPaintEvent& event );
	void OnColEndDrag( wxListEvent& event );
    void OnScrollWin( wxScrollWinEvent& event );

	void FilterEx( vector<wxString>& strings, int flags );

protected:
	unFilterListCtrl*	Finder;	

private:
	DECLARE_EVENT_TABLE()	
};


// ============================================================================
// unFilterListCtrl
// ============================================================================
class unFilterListCtrl : public wxPanel
{
public:
	friend class unFilterList;
	friend class unPackageView;

public:
	unFilterListCtrl
		(	wxWindow*		parent
		,	wxWindowID		id = wxID_ANY
		,	const wxPoint&	pos = wxDefaultPosition
		,	const wxSize&	size = wxDefaultSize
		,	long			style = wxTAB_TRAVERSAL | wxCLIP_CHILDREN
		,	const wxString&	name = wxT("unFilterListCtrl")
		);

	void OnTextEnter( wxCommandEvent& event );
	void OnText( wxCommandEvent& event );
	void OnSize( wxSizeEvent& event );
	void OnContextMenu( wxContextMenuEvent& event );

	void OnMenuCaseSensitive			( wxCommandEvent& event );
	void OnMenuBackReferences			( wxCommandEvent& event );
	void OnMenuSyntax					( wxCommandEvent& event );

	int	GetRegExFlags();
	void UpdateBoxes();

protected:
	unFilterList*			List;	

	wxPanel*				EditPanel;
	vector<wxTextCtrl*>		EditBoxes;
	vector<wxRect>			EditSizes;

	static bool				reCaseSensitive;
	static bool				reBackReferences;
	static int				reSyntax;

private:
	DECLARE_EVENT_TABLE()
};

#endif UNFILTERLIST_H
// ============================================================================
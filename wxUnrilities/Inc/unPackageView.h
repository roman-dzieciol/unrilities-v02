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
//	unPackageView.h
// ============================================================================
#ifndef UNPACKAGEVIEW_H
#define UNPACKAGEVIEW_H

#include "unNotebookPage.h"

class pkgSerializer;
class unFilterListCtrl;
class unVirtualList;
class unTable;


// ============================================================================
// unPackageView
// ============================================================================
class unPackageView : public unNotebookPage
{
public:
	unPackageView
	(	wxWindow*		parent
	,	wxWindowID		id = wxID_ANY
	,	const wxPoint&	pos = wxDefaultPosition
	,	const wxSize&	size = wxDefaultSize
	,	long			style = wxTAB_TRAVERSAL | wxCLIP_CHILDREN
	,	const wxString&	name = wxT("unPackageView")
	);

public:
	virtual bool Show( bool show = true );
    virtual unNotebookPage* Initialize( pkgSerializer* serializer );
    virtual void OnPageChanged();

protected:
	virtual void InitData( unTable& );
    virtual void InitList( unVirtualList&, unTable& );

protected:
	unVirtualList* GetList();
	unTable* GetTable();

protected:
	unFilterListCtrl*	Filter;			// List with regexp bar
	pkgSerializer*		Serializer;		//

private:
//	DECLARE_EVENT_TABLE()
};


#endif UNPACKAGEVIEW_H
// ============================================================================
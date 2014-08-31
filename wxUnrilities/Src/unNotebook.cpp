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
//	unNotebook.cpp
// ============================================================================
#include "prec.h"
#include "unNotebook.h"

#include "unNotebookPage.h"
#include "pkgSerializer.h"
#include "unPackageView.h"
#include "pvNames.h"
#include "pvExports.h"
#include "pvImports.h"
#include "pvTables.h"
#include "ptExports.h"
#include "ptImports.h"


BEGIN_EVENT_TABLE(unNotebook, wxNotebook)
	EVT_NOTEBOOK_PAGE_CHANGED	( wxID_ANY, unNotebook::OnPageChanged )
END_EVENT_TABLE()


unNotebook::unNotebook
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	Serializer(NULL)
,	bClosing(false)
,	wxNotebook(parent,id,pos,size,style,name)
{
}

void unNotebook::OpenFile( pkgSerializer* serializer )
{
	Serializer = serializer;

	Show(false);

	unPackageView* tables = new pvTables(this);
	unPackageView* names = new pvNames(this);
	unPackageView* exports = new pvExports(this);
	unPackageView* imports = new pvImports(this);
	unPackageTree* exporttree = new ptExports(this);
	unPackageTree* importtree = new ptImports(this);

	AddPage( tables, wxT("Tables"), false, -1 );
	AddPage( names, wxT("Names"), false, -1 );
	AddPage( exports, wxT("Exports"), false, -1 );
	AddPage( imports, wxT("Imports"), false, -1 );
	AddPage( exporttree, wxT("Export Tree"), false, -1 );
	AddPage( importtree, wxT("Import Tree"), false, -1 );

	//SetSelection(0);
	Show(true);
}

void unNotebook::AddPage( unNotebookPage* page, const wxString& text, bool bSelect, int imageId )
{
	page->Show(false);
	page->Initialize(Serializer);
	wxNotebook::AddPage(page,text,bSelect,imageId);
}

void unNotebook::CloseFile()
{
	Show(false);
	bClosing = true;

	while( GetPageCount() > 0 )
	{
		wxNotebookPage* page = GetPage(0);
		RemovePage(0);
		page->Show(false);
		page->Destroy();
	}

	Serializer = NULL;
	bClosing = false;
	Show(true);
}

void unNotebook::OnPageChanged( wxNotebookEvent& event )
{
	if( !bClosing )
	{
		//wxLogMessage( "OnPageChanged %d", event.GetSelection() );
		unNotebookPage* page = static_cast<unNotebookPage*>(GetPage(event.GetSelection()));
		page->OnPageChanged();
	}
	event.Skip();
}

// ============================================================================
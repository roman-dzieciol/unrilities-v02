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
//	unPackageTree.cpp
// ============================================================================
#include "prec.h"
#include "unPackageTree.h"


#include "unTable.h"
#include "unTreeList.h"
#include "unDataTree.h"


typedef multimap<wxChar,unDataTree*>	NameMap;
typedef NameMap::iterator				NameMapItr;
typedef NameMap::const_iterator			NameMapCItr;
typedef pair<NameMapItr,NameMapItr>		NameMapRng;

BEGIN_EVENT_TABLE		( unPackageTree, unNotebookPage )
	EVT_COMBOBOX		( wxID_ANY, unPackageTree::OnComboBox )
END_EVENT_TABLE()
 

// TODO::
// - Add plugin interface for display modes


// ============================================================================
// unPackageTree
// ============================================================================
unPackageTree::unPackageTree
(	wxWindow*		parent
,	wxWindowID		id
,	const wxPoint&	pos
,	const wxSize&	size
,	long			style
,	const wxString&	name
) 
:	DataTree(NULL)
,	unNotebookPage(parent,id,pos,size,style,name)
{
}

unPackageTree::~unPackageTree()
{
	Clear();
}

unNotebookPage* unPackageTree::Initialize( pkgSerializer* serializer )
{
	Serializer = serializer;

	InitDisplayModes();

	// controls
	ModeCombo = new wxComboBox( this, wxID_ANY, ModeNames[0], wxDefaultPosition, wxDefaultSize, ModeNames, wxCB_READONLY );
	Tree = new unTreeList(this);

	// sizer
	wxBoxSizer* vs = new wxBoxSizer( wxVERTICAL );
	vs->Add( ModeCombo, 0, wxEXPAND | wxALIGN_TOP, 0 );	
	vs->Add( Tree, 1, wxEXPAND, 0 );	
	vs->SetSizeHints( this );
	SetSizerAndFit( vs ); 

	// init
	InitListTable( GetTable() );
	InitTreeList();

	SetDisplayMode( 0 );

	return this;
}

void unPackageTree::InitDisplayModes()
{
	// init in subclass
}

void unPackageTree::InitListTable( unTable& WXUNUSED(table) )
{	
	// init in subclass
}


void unPackageTree::InitTreeData( dword WXUNUSED(mode) )
{
	// init in subclass
}

void unPackageTree::InitTreeList()
{	
	const unTableHeader& h( GetTable().GetHeader() );
	for( unTableHeader::const_iterator it=h.begin(); it!=h.end(); ++it )
	{
		Tree->AddColumn( (*it).GetCaption(), (*it).GetWidth(), (*it).GetAlign() );
	}
	//Tree->SetVerticalAlign( true );
	Tree->Refresh();
}

void unPackageTree::OnPageChanged()
{
    Tree->SetFocus();
	Tree->ExpandRoot();
	Tree->Refresh();
}


void unPackageTree::DestroyDataTree()
{
	delete DataTree;
	DataTree = NULL;
}

void unPackageTree::Clear()
{
	//SW_DEF( sw );
	Tree->DeleteRoot();
	DestroyDataTree();
	Serializer = NULL;
	//SW_LOGF( sw );
}


void unPackageTree::OnComboBox( wxCommandEvent& event )
{
	SetDisplayMode( event.GetInt() );
}

void unPackageTree::SetDisplayMode( dword mode )
{	
	//SW_DEF( sw );
	DestroyDataTree();	
	InitTreeData( mode );	

	Tree->DeleteRoot();		
	Tree->SetDataTree( DataTree );
	Tree->SetMainColumn( MainColumn );
	Tree->ExpandRoot();
	Tree->Refresh();
	//SW_LOGF( sw );
}

unTable& unPackageTree::GetTable()
{
	return *Tree->Table;
}




// ============================================================================
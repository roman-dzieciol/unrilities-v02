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
//	ptImports.cpp
// ============================================================================
#include "prec.h"
#include "ptImports.h"


#include "unTable.h"
#include "pkgSerializer.h"
#include "unDataTreeFactories.h"

// ============================================================================
// ETreeDisplayModes
// ============================================================================
enum ETreeDisplayModes
{
	TDM_AllTree,
	TDM_AllClasses,
	TDM_Max
};

// ============================================================================
// ptImports
// ============================================================================
ptImports::ptImports
(	wxWindow*		parent
 ,	wxWindowID		id
 ,	const wxPoint&	pos
 ,	const wxSize&	size
 ,	long			style
 ,	const wxString&	name
 ) 
 :	unPackageTree(parent,id,pos,size,style,name)
{
}

void ptImports::InitDisplayModes()
{
	ModeNames.Insert(wxEmptyString, 0, TDM_Max);
	ModeNames[TDM_AllTree]			= wxT("All - Package Tree");
	ModeNames[TDM_AllClasses]		= wxT("All - Class List");
}


void ptImports::InitListTable( unTable& table )
{	
	table << thColumn( wxT("#"),			Fmt::Index )	<< new_printer( tf_Stream(), ds_ImportTable(ItemIndex), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(ObjectName), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("ObjectName"),	Fmt::Name )		<< new_printer( tf_Name(), ds_ImportTable(ObjectName), ic_TextNoCase() ) << new unRegEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(Package), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("Package"),		Fmt::Group )	<< new_printer( tf_ObjRef(), ds_ImportTable(Package), ic_TextNoCase() ) << new unRegEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(ClassPackage), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("ClassPackage"),	Fmt::Group )	<< new_printer( tf_Name(), ds_ImportTable(ClassPackage), ic_TextNoCase() ) << new unRegEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(Class), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("ClassName"),	Fmt::Group )	<< new_printer( tf_Name(), ds_ImportTable(Class), ic_TextNoCase() ) << new unRegEx;

	table.Init( Serializer->GetImportTable().size() );
	MainColumn = 2;
}


void ptImports::InitTreeData( dword mode )
{
	//SW_DEF( sw );

	DataTree = new unDataTree( ModeNames[mode], ID_IGNORE, true );

	switch( mode )
	{
	case TDM_AllTree:		ModeAllTree();		break;
	case TDM_AllClasses:	ModeAllClasses();	break;
	}

	//SW_LOGF( sw );
}

void ptImports::ModeAllTree()
{
	for( pkgImportItem::const_iterator it=Serializer->GetImportRoot().begin(); it!=Serializer->GetImportRoot().end(); ++it )
	{
		pkgImportItem* item = static_cast<pkgImportItem*>(*it);
		unDataTree* tree = DataTree->AddChild( new unDataTree( Serializer->GetName(item->ObjectName), item->ItemIndex, true ) );	
		if( !item->empty() )
			unPkgDataTreeFactory( Serializer, tree, item );
	}
}

void ptImports::ModeAllClasses()
{
	/*pkgImportItem group;
	for( pkgImportItem::const_iterator it=Serializer->GetImportRoot().begin(); it!=Serializer->GetImportRoot().end(); ++it )
	{
		pkgImportItem* item = static_cast<pkgImportItem*>(*it);
		if( item->Class != 0 )
		{
			for( pkgImportItem::iterator classit=item->begin(); classit!=item->end(); ++classit )
			{
				pkgImportItem* classitem = static_cast<pkgImportItem*>(*classit);
				if( classitem->Class != 0 )
					group.AddDescendant( classitem );	
			}
		}
		else
		{
			group.AddDescendant( item );
		}
	}*/
    CreateClassMapper( Serializer, DataTree, &Serializer->GetImportRoot(), &pkgObjectItem::Class, &pkgSerializer::GetName );
}


// ============================================================================
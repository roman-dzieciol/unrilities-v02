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
//	ptExports.cpp
// ============================================================================
#include "prec.h"
#include "ptExports.h"

#include "unTable.h"
#include "pkgSerializer.h"
#include "unDataTreeFactories.h"
#include "unTreeList.h"
#include "pkgObject.h"


// ============================================================================
// ETreeDisplayModes
// ============================================================================
enum ETreeDisplayModes
{
	TDM_AllTree,
	TDM_AllClasses,
	TDM_ScriptTree,
	TDM_ScriptClasses,
	TDM_ScriptInheritance,
	TDM_DataTree,
	TDM_DataClasses,
	TDM_Max
};


enum
{
    Menu_InspectObject = wxID_HIGHEST + 1
};

BEGIN_EVENT_TABLE		( ptExports, unPackageTree )
    EVT_CONTEXT_MENU	( ptExports::OnContextMenu ) 
    EVT_MENU			( Menu_InspectObject,	ptExports::OnMenuInspectObject )
END_EVENT_TABLE()


// ============================================================================
// ptExports
// ============================================================================
ptExports::ptExports
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

void ptExports::InitDisplayModes()
{
	ModeNames.Insert(wxEmptyString, 0, TDM_Max);
	ModeNames[TDM_AllTree]			= wxT("All - Package Tree");
	ModeNames[TDM_AllClasses]		= wxT("All - Class List");
	ModeNames[TDM_ScriptTree]		= wxT("Scripts - Package Tree");
	ModeNames[TDM_ScriptClasses]	= wxT("Scripts - Class List");
	ModeNames[TDM_ScriptInheritance]= wxT("Scripts - Inheritance Tree");
	ModeNames[TDM_DataTree]			= wxT("Data - Package Tree");
	ModeNames[TDM_DataClasses]		= wxT("Data - Class List");
}


void ptExports::InitListTable( unTable& table )
{	
	table << thColumn( wxT("#"),			Fmt::Index )	<< new_printer( tf_Stream(), ds_ExportTable(ItemIndex), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(ObjectName), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("ObjectName"),	Fmt::Name )		<< new_printer( tf_Name(), ds_ExportTable(ObjectName), ic_TextNoCase() ) << new unRegEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(Package), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("Package"),		Fmt::Group )	<< new_printer( tf_ObjRef(), ds_ExportTable(Package), ic_TextNoCase() ) << new unRegEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(Class), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("Class"),		Fmt::Name )		<< new_printer( tf_ObjRef(), ds_ExportTable(Class), ic_TextNoCase() ) << new unRegEx;
	table << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(Super), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("Super"),		Fmt::Group )	<< new_printer( tf_ObjRef(), ds_ExportTable(Super), ic_TextNoCase() ) << new unRegEx;
	table << thColumn( wxT("SerialSize"),	Fmt::Size )		<< new_printer( tf_Stream(), ds_ExportTable(SerialSize), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("SerialOffset"),	Fmt::Offset )	<< new_printer( tf_Offset(), ds_ExportTable(SerialOffset), ic_Data() ) << new unMathEx;
	table << thColumn( wxT("ObjectFlags"),	Fmt::Flag )		<< new_printer( tf_Flags(GFlag.GetGroup("ObjectFlags")), ds_ExportTable(ObjectFlags), ic_Data() ) << new unRegEx;

	table.Init( Serializer->GetExportTable().size() );
	MainColumn = 2;
}


void ptExports::InitTreeData( dword mode )
{
	//SW_DEF( sw );

	DataTree = new unDataTree( ModeNames[mode], ID_IGNORE, true );

	switch( mode )
	{
	case TDM_AllTree:				ModeAllTree();				break;
	case TDM_AllClasses:			ModeAllClasses();			break;
	case TDM_ScriptTree:			ModeScriptTree();			break;
	case TDM_ScriptClasses:			ModeScriptClasses();		break;
	case TDM_ScriptInheritance:		ModeScriptInheritance();	break;
	case TDM_DataTree:				ModeDataTree();				break;
	case TDM_DataClasses:			ModeDataClasses();			break;
	}

	//SW_LOGF( sw );
}

void ptExports::ModeAllTree()
{
	for( pkgExportItem::const_iterator it=Serializer->GetExportRoot().begin(); it!=Serializer->GetExportRoot().end(); ++it )
	{
		pkgExportItem* item = static_cast<pkgExportItem*>(*it);
		if( !item->IsNull() )
		{
			unDataTree* tree = DataTree->AddChild( new unDataTree( Serializer->GetName(item->ObjectName), item->ItemIndex, true ) );	
			if( !item->empty() )
				unPkgDataTreeFactory( Serializer, tree, item );
		}
	}
}

void ptExports::ModeAllClasses()
{
	pkgExportItem group;
	for( pkgExportItem::const_iterator it=Serializer->GetExportRoot().begin(); it!=Serializer->GetExportRoot().end(); ++it )
	{
		pkgExportItem* item = static_cast<pkgExportItem*>(*it);
		if( !item->IsNull() )
		{
			if( item->Class == 0 )
			{
				for( pkgExportItem::iterator classit=item->begin(); classit!=item->end(); ++classit )
				{
					pkgExportItem* classitem = static_cast<pkgExportItem*>(*classit);
					if( !classitem->IsNull() )
					{
						if( classitem->Class != 0 )
							group.AddDescendant( classitem );	
					}
				}
			}
			else
			{
				group.AddDescendant( item );
			}
		}
	}
    CreateClassMapper( Serializer, DataTree, &group, &pkgObjectItem::ObjectName, &pkgSerializer::GetObjectName );
}

void ptExports::ModeScriptTree()
{
	for( pkgExportItem::const_iterator it=Serializer->GetExportRoot().begin(); it!=Serializer->GetExportRoot().end(); ++it )
	{
		pkgExportItem* item = static_cast<pkgExportItem*>(*it);
		if( !item->IsNull() )
		{
			if( item->Class == 0 )
			{
				unDataTree* tree = DataTree->AddChild( new unDataTree( Serializer->GetName(item->ObjectName), item->ItemIndex, true ) );	
				if( !item->empty() )
					unPkgDataTreeFactory( Serializer, tree, item );
			}
		}
	}
}

void ptExports::ModeScriptClasses()
{
	pkgExportItem group;
	for( pkgExportItem::const_iterator it=Serializer->GetExportRoot().begin(); it!=Serializer->GetExportRoot().end(); ++it )
	{
		pkgExportItem* item = static_cast<pkgExportItem*>(*it);
		if( !item->IsNull() )
		{
			if( item->Class == 0 )
			{
				for( pkgExportItem::iterator classit=item->begin(); classit!=item->end(); ++classit )
				{
					pkgExportItem* classitem = static_cast<pkgExportItem*>(*classit);
					if( !classitem->IsNull() )
					{
						if( classitem->Class != 0 )
							group.AddDescendant( classitem );	
					}
				}
			}
		}
	}
	CreateClassMapper( Serializer, DataTree, &group, &pkgObjectItem::ObjectName, &pkgSerializer::GetObjectName );
}

void ptExports::ModeScriptInheritance()
{
	pkgExportItem group;
	for( pkgExportItem::const_iterator it=Serializer->GetExportRoot().begin(); it!=Serializer->GetExportRoot().end(); ++it )
	{
		pkgExportItem* item = static_cast<pkgExportItem*>(*it);
		if( !item->IsNull() )
		{
			if( item->Class == 0 )
				group.AddDescendant( item );	
		}
	}
	unInheritanceMapper(Serializer,DataTree,&group);
}

void ptExports::ModeDataTree()
{
	for( pkgExportItem::const_iterator it=Serializer->GetExportRoot().begin(); it!=Serializer->GetExportRoot().end(); ++it )
	{
		pkgExportItem* item = static_cast<pkgExportItem*>(*it);
		if( !item->IsNull() )
		{
			if( item->Class != 0 )
			{
				unDataTree* tree = DataTree->AddChild( new unDataTree( Serializer->GetName(item->ObjectName), item->ItemIndex, true ) );	
				if( !item->empty() )
					unPkgDataTreeFactory( Serializer, tree, item );
			}
		}
	}
}

void ptExports::ModeDataClasses()
{
	pkgExportItem group;
	for( pkgExportItem::const_iterator it=Serializer->GetExportRoot().begin(); it!=Serializer->GetExportRoot().end(); ++it )
	{
		pkgExportItem* item = static_cast<pkgExportItem*>(*it);
		if( !item->IsNull() )
		{
			if( item->Class != 0 )
				group.AddDescendant( item );
		}
	}
	CreateClassMapper( Serializer, DataTree, &group, &pkgObjectItem::ObjectName, &pkgSerializer::GetObjectName );
}


void ptExports::OnContextMenu( wxContextMenuEvent& event )
{
    wxTreeItemId item = Tree->GetSelection();

    if( item.IsOk() )
    {
        unDataTree* tree = Tree->GetDataTree(item);

        wxPoint pt = event.GetPosition();
        if( pt.x == -1 && pt.y == -1 ) 
        {
            wxRect rect;
            Tree->GetBoundingRect(item,rect);
            pt = rect.GetPosition();
        }
        else 
        {
            pt = ScreenToClient(pt);
        }

        wxString caption;
        caption = tree->GetCaption();

        wxMenu menu(caption);
        menu.Append(Menu_InspectObject, wxT("&Inspect Object"));
        //menu.AppendSeparator();

        PopupMenu(&menu, pt);
    }
}

void ptExports::OnMenuInspectObject( wxCommandEvent& WXUNUSED(event) )
{  
    wxTreeItemId item = Tree->GetSelection();

    if( item.IsOk() )
    {
        unDataTree* tree = Tree->GetDataTree(item);
        if( tree->GetSerialID() != ID_IGNORE )
        {
            pkgObject obj(Serializer,tree->GetSerialID());
            obj.LoadObject();
            obj.DumpProperties();
        }
    }

}

// ============================================================================
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
//	pvExports.cpp
// ============================================================================
#include "prec.h"
#include "pvExports.h"

#include "unTable.h"
#include "pkgSerializer.h"
#include "unVirtualList.h"
#include "pkgObject.h"



enum
{
    Menu_InspectObject = wxID_HIGHEST + 1,
    Menu_InspectAll
};

BEGIN_EVENT_TABLE		( pvExports, unPackageView )
    EVT_CONTEXT_MENU	( pvExports::OnContextMenu ) 
    EVT_MENU			( Menu_InspectObject,	pvExports::OnMenuInspectObject )
    EVT_MENU			( Menu_InspectAll,	    pvExports::OnMenuInspectAll )
END_EVENT_TABLE()


pvExports::pvExports
(	wxWindow*		parent
 ,	wxWindowID		id
 ,	const wxPoint&	pos
 ,	const wxSize&	size
 ,	long			style
 ,	const wxString&	name
 ) 
 :	unPackageView(parent,id,pos,size,style,name)
{
}


void pvExports::InitData( unTable& vtable )
{
	vtable << thColumn( wxT("#"),				Fmt::Index )	<< new_printer( tf_Stream(), ds_ExportTable(ItemIndex), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(ObjectName), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("ObjectName"),		Fmt::Name )		<< new_printer( tf_Name(), ds_ExportTable(ObjectName), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(Package), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("Package"),			Fmt::Group )	<< new_printer( tf_ObjRef(), ds_ExportTable(Package), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(Class), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("Class"),			Fmt::Name )		<< new_printer( tf_ObjRef(), ds_ExportTable(Class), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ExportTable(Super), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("Super"),			Fmt::Group )	<< new_printer( tf_ObjRef(), ds_ExportTable(Super), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("SerialSize"),		Fmt::Size )		<< new_printer( tf_Stream(), ds_ExportTable(SerialSize), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("SerialOffset"),	Fmt::Offset )	<< new_printer( tf_Offset(), ds_ExportTable(SerialOffset), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("ObjectFlags"),		Fmt::Flag )		<< new_printer( tf_Flags(GFlag.GetGroup("ObjectFlags")), ds_ExportTable(ObjectFlags), ic_Data() ) << new unRegEx;

	vtable.Init( Serializer->GetExportTable().size() );
}


void pvExports::OnContextMenu( wxContextMenuEvent& event )
{
    wxPoint pt = event.GetPosition();
    if( pt.x == -1 && pt.y == -1 ) 
    {
        pt = wxWindow::FindFocus()->GetPosition();
    }
    else 
    {
        pt = ScreenToClient(pt);
    }

    long item = GetList()->GetFocusedItem();
    if( item != -1 )
    {
        wxString caption;
        caption = Serializer->GetObjectName( GetTable()->GetSerialIdx(item)+1 );

        wxMenu menu(caption);
        menu.Append(Menu_InspectObject, wxT("&Inspect Object"));
        menu.Append(Menu_InspectAll, wxT("Inspect &All (Debug)"));
        //menu.AppendSeparator();

        PopupMenu(&menu, pt);
    }
         


}

void pvExports::OnMenuInspectObject( wxCommandEvent& WXUNUSED(event) )
{  
    long item = GetList()->GetFocusedItem();
    if( item != -1 )
    {
        pkgObject obj(Serializer,GetTable()->GetSerialIdx(item));
        obj.LoadObject();
        obj.DumpProperties();
    }
}

void pvExports::OnMenuInspectAll( wxCommandEvent& WXUNUSED(event) )
{  
    long item = GetList()->GetFocusedItem();
    if( item != -1 )
    {
        wxLogMessage( wxT("") );
        for( dword i=0; i<Serializer->GetExportTable().size(); ++i )
        {
            pkgObject obj(Serializer,i);
            obj.LoadObject();
            obj.DumpProperties();
        }
    }
}


// ============================================================================
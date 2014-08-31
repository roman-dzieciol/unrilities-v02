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
//	pvImports.cpp
// ============================================================================
#include "prec.h"
#include "pvImports.h"

#include "unTable.h"
#include "unFilterList.h"
#include "pkgSerializer.h"


pvImports::pvImports
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


void pvImports::InitData( unTable& vtable )
{
	vtable << thColumn( wxT("#"),				Fmt::Index )	<< new_printer( tf_Stream(), ds_ImportTable(ItemIndex), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(ObjectName), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("ObjectName"),		Fmt::Name )		<< new_printer( tf_Name(), ds_ImportTable(ObjectName), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(Package), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("Package"),			Fmt::Group )	<< new_printer( tf_ObjRef(), ds_ImportTable(Package), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(ClassPackage), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("ClassPackage"),	Fmt::Group )	<< new_printer( tf_Name(), ds_ImportTable(ClassPackage), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("# of"),			Fmt::IndexOf )	<< new_printer( tf_Stream(), ds_ImportTable(Class), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("ClassName"),		Fmt::Group )	<< new_printer( tf_Name(), ds_ImportTable(Class), ic_TextNoCase() ) << new unRegEx;

	vtable.Init( Serializer->GetImportTable().size() );
}

// ============================================================================
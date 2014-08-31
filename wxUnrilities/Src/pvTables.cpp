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
//	pvTables.cpp
// ============================================================================
#include "prec.h"
#include "pvTables.h"

#include "unTable.h"
#include "pkgSerializer.h"


pvTables::pvTables
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


void pvTables::InitData( unTable& vtable )
{
	vtable << thColumn( wxT("Name"), Fmt::Name )		<< new_printer( tf_Stream(), ds_Tables(Name), ic_TextNoCase() ) << new unRegEx;
	vtable << thColumn( wxT("Size"), Fmt::Size )		<< new_printer( tf_Stream(), ds_Tables(Size), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("Items"), Fmt::Size )		<< new_printer( tf_Stream(), ds_Tables(Count), ic_Data() ) << new unMathEx;
	vtable << thColumn( wxT("Offset"), Fmt::Offset )	<< new_printer( tf_Offset(), ds_Tables(Offset), ic_Data() ) << new unMathEx;

	vtable.Init( Serializer->GetTableCount() );
}

// ============================================================================
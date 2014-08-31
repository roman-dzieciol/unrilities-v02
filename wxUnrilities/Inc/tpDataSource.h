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
//	tpDataSource.h
// ============================================================================
#ifndef TPDATASOURCE_H
#define TPDATASOURCE_H

#include "tpTemplate.h"
#include "pkgSerializer.h"



// ============================================================================
// DataSource :: ds_Data
// ============================================================================
struct ds_Data
{
	template<class T>
	dword& GetData( T& item, dword )
	{
		return item.Data;
	}
};



// ============================================================================
// DataSource :: ds_Serialized
// ============================================================================
// TT = Package table
// IT = Table item
// VT = Member variable
// ST = Serializer
// ============================================================================
template <class TT, class IT, class VT, class ST=pkgSerializer> 
struct ds_Serialized
{
	ds_Serialized( ST& s, TT ST::*pmt, VT IT::*pmv  ) 
		: Serializer(s), Table(pmt), Variable(pmv) {}

		template<class T>
		VT& GetData( T&, dword row )
		{
			return ((Serializer.*Table) AT(row).*Variable);
		}

		ST& Serializer;
		TT ST::*Table;
		VT IT::*Variable;
};


// ============================================================================
// Creator :: ds_Serialized
// ============================================================================
// TT = Package table
// IT = Table item
// VT = Member variable
// ST = Serializer
// ============================================================================
template <class TT, class IT, class VT, class ST> 
ds_Serialized<TT,IT,VT> make_serialized( ST& s, TT ST::*pmt, VT IT::*pmv )
{ 
	return ds_Serialized<TT,IT,VT,ST>(s,pmt,pmv); 
}


// ============================================================================
// Aliases :: ds_Serialized
// ============================================================================
#define ds_NameTable(MV)		make_serialized(*Serializer, &pkgSerializer::NameTable, &pkgNameItem::MV)
#define ds_ExportTable(MV)		make_serialized(*Serializer, &pkgSerializer::ExportTable, &pkgExportItem::MV)
#define ds_ImportTable(MV)		make_serialized(*Serializer, &pkgSerializer::ImportTable, &pkgImportItem::MV)
#define ds_Tables(MV)			make_serialized(*Serializer, &pkgSerializer::Tables, &pkgTableHeader::MV)






#endif TPDATASOURCE_H
// ============================================================================
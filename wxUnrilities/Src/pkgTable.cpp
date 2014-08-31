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
//	pkgTable.cpp
// ============================================================================
#include "prec.h"
#include "pkgTable.h"

void pkgExportTable::InitTable()
{
	SW_DEF( sw );
	for( iterator it=begin(); it!=end(); ++it )
	{
		pkgExportItem* item = &(*it);	

		// find parent
		if( item->Package > 0 )
		{
			(*this) AT(item->Package-1).AddChild( item );
		}
		else if( item->Package == 0 )
		{
			Root.AddChild( item );
		}
		else
		{
			wxString err;
			err.Printf( wxT("ExportItem[%d] package is in import tree!"), dword(it-begin()) );
			throw exFail( err.wx_str() );
		}
	}
	SW_LOGF( sw );
}

void pkgImportTable::InitTable()
{
	SW_DEF( sw );
	for( iterator it=begin(); it!=end(); ++it )
	{
		pkgImportItem* item = &(*it);	

		// find parent
		if( item->Package < 0 )
		{
			(*this) AT(-item->Package-1).AddChild( item );
		}
		else if( item->Package == 0 )
		{
			Root.AddChild( item );
		}
		else
		{
			wxString err;
			err.Printf( wxT("ImportItem[%d] package is in export tree!"), dword(it-begin()) );
			throw exFail( err.wx_str() );
		}
	}
	SW_LOGF( sw );
}

// ============================================================================
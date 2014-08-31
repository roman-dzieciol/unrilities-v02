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
//	pkgSerializer.cpp
// ============================================================================
#include "prec.h"
#include "pkgSerializer.h"


const wxString	pkgSerializer::NameInvalid = wxT("INVALID");
const wxString	pkgSerializer::NameNull = wxT("");
const wxString	pkgSerializer::NameSeparator = wxT(".");


void pkgSerializer::OpenFile( wxFileDialog& dialog )
{
	SW_DEF( sw );

	FilePath = dialog.GetPath();
	FileName = dialog.GetFilename();
	FileDir = dialog.GetDirectory();


	File.Open( FilePath, wxT("rb") );

	File >> Header;

	File >> static_cast<pkgTableHeader&>(NameTable);
	File >> static_cast<pkgTableHeader&>(ExportTable);
	File >> static_cast<pkgTableHeader&>(ImportTable);

	File >> NameTable;
	File >> ExportTable;
	File >> ImportTable;


	Tables.push_back( static_cast<pkgTableHeader>(NameTable) );	
	Tables.push_back( static_cast<pkgTableHeader>(ExportTable) );
	Tables.push_back( static_cast<pkgTableHeader>(ImportTable) );

	SW_LOGF( sw );
}

wxString pkgSerializer::GetObjectName( int32 index ) const
{
	wxString name;

	for( int i=0; i<NameRecursion; ++i )
	{
		if( index > 0 )	
		{
			const pkgExportItem& item = ExportTable AT(index-1);
			name.Prepend( GetName(item.ObjectName) );
			index = item.Package;
		}
		else if( index < 0 )	
		{
			const pkgImportItem& item = ImportTable AT(-index-1);	
			name.Prepend( GetName(item.ObjectName) );
			index = item.Package;
		}
		else
		{
			name.Prepend( NameNull );
			index = 0;
		}

		if( index != 0 )
		{
			name.Prepend( NameSeparator );
		}
		else
		{
			break;
		}
	}

	return name;
}


// ============================================================================
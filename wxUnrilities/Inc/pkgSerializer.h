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
//	pkgSerializer.h
// ============================================================================
#ifndef PKGSERIALIZER_H
#define PKGSERIALIZER_H

#include "pkgArchive.h"
#include "pkgTable.h"
#include "pkgHeader.h"


// ============================================================================
// pkgSerializer
// ============================================================================
class pkgSerializer
{
public:
    friend class pkgObject;
    friend class pkgProperty;

public:
	pkgSerializer()
	{
	}

	~pkgSerializer()
	{
		if( File.IsOpened() )
			File.Close();
	}

public:
	void OpenFile( wxFileDialog& dialog );

public:
	wxString GetObjectName( int32 index ) const;

	wxString GetClassName( int32 idx ) const
	{
		if		( idx < 0 )	return GetName( ImportTable AT(-idx-1).ObjectName.GetData() );
		else if	( idx > 0 )	return GetName( ExportTable AT(idx-1).ObjectName.GetData() );
		else				return NameInvalid;		
	}	

	dword Translate( int32 idx ) const	
	{ 
		if		( idx < 0 )	return -idx-1;
		else if	( idx > 0 )	return idx-1;
		else				return 0;
	}

public:
	wxFileOffset			GetFileSize	() const			{ return File.Length(); }
	wxString				GetFileDir	() const			{ return FileDir; }
	wxString				GetFileName	() const			{ return FileName; }
	wxString				GetFilePath	() const			{ return FilePath; }
public:
	const pkgHeader&		GetHeader() const				{ return Header; }

	const pkgNameTable&		GetNameTable() const			{ return NameTable; }
	const pkgExportTable&	GetExportTable() const			{ return ExportTable; }
	const pkgImportTable&	GetImportTable() const			{ return ImportTable; }

    const pkgNameItem&		GetNameItem( dword i ) const	{ return NameTable AT(i); }
    const pkgExportItem&	GetExportItem( dword i ) const	{ return ExportTable AT(i); }
    const pkgImportItem&	GetImportItem( dword i ) const	{ return ImportTable AT(i); }

	const pkgImportItem&	GetImportRoot() const			{ return ImportTable.Root; }
	const pkgExportItem&	GetExportRoot() const			{ return ExportTable.Root; }

	const pkgTableHeader&	GetTable( dword i ) const		{ return Tables AT(i); }
	const dword				GetTableCount() const			{ return Tables.size(); }

	wxString				GetName( int32 idx ) const		{ return NameTable AT(idx).Name; }

protected:
    operator pkgArchive&() { return File; }

protected:
	pkgArchive				File;				//
	wxString				FileDir;			//
	wxString				FileName;			//
	wxString				FilePath;			//

protected:
	pkgHeader				Header;				//

public:
	vector<pkgTableHeader>	Tables;				// Tables list
	pkgNameTable			NameTable;			// Name table
	pkgExportTable			ExportTable;		// Export table
	pkgImportTable			ImportTable;		// Import table

public:
	static const wxString	NameInvalid;		//
	static const wxString	NameNull;			//
	static const wxString	NameSeparator;		//
	static const dword		NameRecursion = 7;	//
};




#endif PKGSERIALIZER_H
// ============================================================================
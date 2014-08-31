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
//	pkgTable.h
// ============================================================================
#ifndef PKGTABLE_H
#define PKGTABLE_H

#include "pkgArchive.h"
#include "pkgTableItem.h"


// ============================================================================
// pkgTableHeader
// ============================================================================
class pkgTableHeader
{
public:
	pkgTableHeader()
	{
	}

	friend pkgArchive& operator >> ( pkgArchive& A, pkgTableHeader& D )
	{
		A >> D.Count;
		A >> D.Offset;
		return A;
	}

public:
	dword		GetSize() const			{ return Size; }
	dword		GetCount() const		{ return Count; }
	wxString	GetName() const			{ return Name; }
	wxString	GetShortName() const	{ return ShortName; }

public:
	dword		Size;
	dword		Count;
	dword		Offset;
	wxString	Name;
	wxString	ShortName;
};


// ============================================================================
// pkgTable
// ============================================================================
template< class T> 
class pkgTable : public vector<T>, public pkgTableHeader
{
public:
	friend pkgArchive& operator >> ( pkgArchive& A, pkgTable<T>& D )
	{
		SW_DEF( sw );
		D.reserve( D.Count );
		D.resize( D.Count );
		A.Seek( D.Offset );
		D.Size = static_cast<dword>(A.Tell());

		for( iterator it=D.begin(); it!=D.end(); ++it )
		{
			(*it).ItemOffset = static_cast<dword>(A.Tell());
			(*it).ItemIndex = it-D.begin();
			A >> (*it);
		}

		D.Size = static_cast<dword>(A.Tell() - D.Size);
		SW_LOGF( sw );
		return A;
	}
};


// ============================================================================
// pkgNameTable
// ============================================================================
class pkgNameTable : public pkgTable<pkgNameItem>
{
public:
	pkgNameTable()
	{
		Name = wxT("NameTable");
		ShortName = wxT("Names");
	}
};

typedef pkgNameTable::iterator			pkgNameTableItr;
typedef pkgNameTable::const_iterator	pkgNameTableCItr;


// ============================================================================
// pkgExportTable
// ============================================================================
class pkgExportTable : public pkgTable<pkgExportItem>
{
public:
	friend class pkgSerializer;

public:
	pkgExportTable()
	{
		Name = wxT("ExportTable");
		ShortName = wxT("Exports");
	}

	friend pkgArchive& operator >> ( pkgArchive& A, pkgExportTable& D )
	{
		A >> static_cast< pkgTable<pkgExportItem>& >(D);
		D.InitTable();
		return A;
	}

protected:
	void InitTable();

protected:
	pkgExportItem Root;
};

typedef pkgExportTable::iterator		pkgExportTableItr;
typedef pkgExportTable::const_iterator	pkgExportTableCItr;


// ============================================================================
// pkgImportTable
// ============================================================================
class pkgImportTable : public pkgTable<pkgImportItem>
{
public:
	friend class pkgSerializer;
		
public:
	pkgImportTable()
	{
		Name = wxT("ImportTable");
		ShortName = wxT("Imports");
	}

	friend pkgArchive& operator >> ( pkgArchive& A, pkgImportTable& D )
	{
		A >> static_cast< pkgTable<pkgImportItem>& >(D);
		D.InitTable();
		return A;
	}

protected:
	void InitTable();

protected:
	pkgImportItem Root;
};

typedef pkgImportTable::iterator		pkgImportTableItr;
typedef pkgImportTable::const_iterator	pkgImportTableCItr;


// ============================================================================
// pkgGenerationTable
// ============================================================================
class pkgGenerationTable : public pkgTable<pkgGenerationItem>
{
public:
	pkgGenerationTable()
	{
		Name = wxT("GenerationTable");
		ShortName = wxT("Generations");
	}

};

typedef pkgGenerationTable::iterator		pkgGenerationTableItr;
typedef pkgGenerationTable::const_iterator	pkgGenerationTableCItr;



#endif PKGTABLE_H
// ============================================================================
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
//	pkgTableItem.h
// ============================================================================
#ifndef PKGTABLEITEM_H
#define PKGTABLEITEM_H

#include "pkgArchive.h"
#include "unTree.h"


// ============================================================================
// pkgTableItem
// ============================================================================
struct pkgTableItem : public unTree<pkgTableItem>
{
	pkgTableItem(){}

	dword	ItemIndex;
	dword	ItemOffset;
};


// ============================================================================
// pkgNameItem
// ============================================================================
struct pkgNameItem : public pkgTableItem
{
	pkgNameItem(){}

	friend pkgArchive& operator >> ( pkgArchive& A, pkgNameItem& D )
	{
		A >> D.Name;
		A >> D.Flags;
		return A;
	}

	// serialized
	unName	Name;
	unFlag	Flags;
};

// ============================================================================
// pkgObjectItem
// ============================================================================
struct pkgObjectItem : public pkgTableItem 
{
    pkgObjectItem(){}

	// serialized
	unIndex		Class;
    int32		Package;
    unIndex		ObjectName;
};


// ============================================================================
// pkgExportItem
// ============================================================================
struct pkgExportItem : public pkgObjectItem
{
	pkgExportItem(){}

	friend pkgArchive& operator >> ( pkgArchive& A, pkgExportItem& D )
	{
		A >> D.Class;
		A >> D.Super;
		A >> D.Package;
		A >> D.ObjectName;
		A >> D.ObjectFlags;
		A >> D.SerialSize;

		if( D.SerialSize > 0 )	A >> D.SerialOffset;
		else					D.SerialOffset = 0;

#if 0	
		wxLogMessage(wxT("pkgExportItem[%d][%X]:, Size: %d, Offset: %X") 
		,D.ItemIndex 
		,D.ItemOffset 
		,D.SerialSize.GetData() 
		,D.SerialOffset.GetData()
		);
#endif

		return A;
	}
	
	bool IsNull()
	{
		return	(	Class == 0 
				&&	Super == 0 
				&&	Package == 0
				&&	ObjectFlags == 0
				&&	SerialSize == 0 
				&&	SerialOffset == 0 );
	}

	// serialized
	unIndex		Super;
	dword		ObjectFlags;
	unIndex		SerialSize;
	unIndex		SerialOffset;
};


// ============================================================================
// pkgImportItem
// ============================================================================
struct pkgImportItem : public pkgObjectItem
{
	pkgImportItem(){}

	friend pkgArchive& operator >> ( pkgArchive& A, pkgImportItem& D )
	{
		A >> D.ClassPackage;
		A >> D.Class;
		A >> D.Package;
		A >> D.ObjectName;
		return A;
	}

	// serialized
	unIndex		ClassPackage;
};


// ============================================================================
// pkgGenerationItem
// ============================================================================
struct pkgGenerationItem : public pkgTableItem 
{
	pkgGenerationItem(){}

	friend pkgArchive& operator >> ( pkgArchive& A, pkgGenerationItem& D )
	{
		A >> D.ExportCount;
		A >> D.NameCount;
		return A;
	}

	// serialized
	dword	ExportCount;
	dword	NameCount;
};






#endif PKGTABLEITEM_H
// ============================================================================
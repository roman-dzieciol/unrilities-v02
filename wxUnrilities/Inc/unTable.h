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
//	unTable.h
// ============================================================================
#ifndef UNTABLE_H
#define UNTABLE_H

#include "unTableData.h"
#include "unTableHeader.h"
#include "unTablePrinter.h"
#include "unTableIndex.h"


// ============================================================================
// unTableFilter
// ============================================================================
struct unTableFilter : public vector<unExpression*>
{
	~unTableFilter()
	{
		clear();
	}

	void clear()
	{
		for( iterator it=begin(); it!=end(); ++it )
		{
			delete static_cast<unExpression*>(*it);
		}
		vector<unExpression*>::clear();
	}
};


// ============================================================================
// unTableSort
// ============================================================================
struct unTableSort
{
	tpPrinter&	Printer;
	tdColumn&	Column;

	unTableSort( tpPrinter& printer, tdColumn& column ) 
	: Printer(printer)
	, Column(column)
	{ 
	}

	bool operator()( dword a, dword b )
	{
		return Printer.Compare( Column AT(a), a, Column AT(b), b );
	}
};


// ============================================================================
// unTable
// ============================================================================
class unTable
{
public:
	~unTable()
	{
		Clear();
	}

public:
	void Clear();
	void Init( dword height );

public:
	unTable& operator << ( const thColumn& column );
	unTable& operator << ( tpPrinter* printer );
	unTable& operator << ( unExpression* filter );

public:
	dword FilterEx( vector<wxString>& strings, int flags );
	void Sort( dword column, bool bforce=false );

public:
	wxString GetText( dword row, dword column )
	{
		row = IdxRegEx AT(row);
		return GetTextDirect( row, column );
	}

	wxString GetTextDirect( dword row, dword column )
	{
		//wxLogMessage("GetTextDirect: %s %d", w.wx_str(), row);
		return GetPrinter(column)->Print( GetCell(row,column), row );
	}

	tdCell& GetCell( dword row, dword column )				{ return Data AT(column) AT(row); }
	tpPrinter* GetPrinter( dword column ) const				{ return Printer AT(column); }
	unExpression* GetFilter( dword column ) const			{ return Filter AT(column); }

public:
	const unTableData&		GetData() const		{ return Data; }
	const unTableHeader&	GetHeader() const	{ return Header; }
	const unTablePrinter&	GetPrinter() const	{ return Printer; }
	const unTableFilter&	GetFilter() const	{ return Filter; }

	dword	GetHeight() const	{ return Data.empty() ? 0 : Data AT(0).size(); }
	dword	GetWidth() const	{ return Header.size(); }
	bool	empty() const		{ return Data.empty(); }
    dword   GetSerialIdx( dword listidx ){ return IdxRegEx AT(listidx); }

protected:
	unTableData			Data;
	unTableHeader		Header;
	unTablePrinter		Printer;
	unTableFilter		Filter;

protected:
	unTableIndex		IdxDefault;
	unTableIndex		IdxRegEx;

protected:
	dword				SortedColumn;
};




#endif UNTABLE_H
// ============================================================================
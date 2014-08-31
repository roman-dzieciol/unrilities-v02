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
//	unTable.cpp
// ============================================================================
#include "prec.h"
#include "unTable.h"




dword unTable::FilterEx( vector<wxString>& strings, int flags )
{
	//SW_DEF( sw );

	unTableIndex table = IdxDefault;

	for( vector<wxString>::iterator it = strings.begin(); it != strings.end(); ++it )
	{
		wxString& str = (*it);
		dword column = dword(it-strings.begin());

		if( str.IsEmpty() )
		{
		}
		else
		{
			unExpression* ex = Filter AT(column);

			ex->Compile( str, flags );

			if( ex->IsValid() )
			{
				dword row;
				unTableIndex newtable;
				tdColumn& cdata = Data AT(column);
				tpPrinter* printer = GetPrinter(column);
				dword tsize = table.size();

				for( dword i=0; i<tsize; ++i )
				{
					row = table AT(i);
					wxString& text = printer->Print( cdata AT(row), row );

					if( ex->Matches( text.wx_str() ) )
					{
						newtable.push_back( row );
					}
					else
					{
					}
				}
				table = newtable;
			}
			else
			{
			}
		}

	}
	IdxRegEx = table;

	//SW_LOG( sw, "Filtered in: %s" );

	Sort( SortedColumn, true );

	return table.size();
}

void unTable::Clear()
{
	Data.clear();
	Header.clear();
	Printer.clear();

	IdxRegEx.clear();
	Filter.clear();
}

void unTable::Init( dword height )
{
	// get column count
	dword width = Header.size();

	// make column template
	tdColumn column;
	column.reserve(height);
	column.resize(height);

	// init columns
	Data.clear();
	Data.reserve(width);
	Data.resize(width, column);

	// create index table	
	IdxDefault.reserve(height);
	IdxDefault.resize(height);
	IdxDefault.FillDefault();

	IdxRegEx = IdxDefault;

	SortedColumn = 0;
}

unTable& unTable::operator << ( const thColumn& column )
{
	Header.push_back( column );
	return *this;
}

unTable& unTable::operator << ( tpPrinter* printer )
{
	Printer.push_back( printer );
	return *this;
}

unTable& unTable::operator << ( unExpression* filter )
{
	Filter.push_back( filter );
	return *this;
}

void unTable::Sort( dword column, bool bforce )
{
	//SW_DEF( sw );

	if( column != SortedColumn || bforce )
	{
		SortedColumn = column;
		stable_sort( IdxRegEx.begin(), IdxRegEx.end(), unTableSort( *Printer AT(column), Data AT(column)  ) );
	}
	else
	{
		reverse( IdxRegEx.begin(), IdxRegEx.end() );
	}

	//SW_VLOG( sw, "Sorted by %s in %s.", Header AT(column).GetCaption().wx_str() );
}

// ============================================================================
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
//	unTableHeader.h
// ============================================================================
#ifndef UNTABLEHEADER_H
#define UNTABLEHEADER_H

#include "unTableData.h"
#include "unExpression.h"

enum unAlignFormat
{
	unFMT_DEFAULT,
	unFMT_LIST
};

// ============================================================================
// thFormat :: Table header format
// ============================================================================
class thFormat
{
public:
	thFormat( dword align = wxLIST_ALIGN_DEFAULT, dword width = -1 )
	: Align(align), Width(width)
	{
	}

public:
	dword	GetWidth() const	{ return Width; }
	
	dword	GetAlign( unAlignFormat format = unFMT_DEFAULT ) const	
	{
		switch(format)
		{
			case unFMT_LIST:
			{
				switch(Align)
				{
				case wxALIGN_LEFT: return wxLIST_FORMAT_LEFT;
				case wxALIGN_RIGHT: return wxLIST_FORMAT_RIGHT;
				case wxALIGN_CENTER: return wxLIST_FORMAT_CENTER;
				}
			}

			case unFMT_DEFAULT:
			default:
				return Align;
		}
	}

protected:
	dword	Align;
	dword	Width;
};


// ============================================================================
// thColumn :: Table header column
// ============================================================================
class thColumn
{
public:
	thColumn(){}
	thColumn( const wxString& caption, const thFormat& format )
		: Caption(caption), Format(format) {}

public:
	const wxString&	GetCaption() const										{ return Caption; }
	dword			GetAlign( unAlignFormat format = unFMT_DEFAULT ) const	{ return Format.GetAlign(format); }
	dword			GetWidth() const										{ return Format.GetWidth(); }

protected:
	wxString	Caption;
	thFormat	Format;	
};


// ============================================================================
// unTableHeader
// ============================================================================
class unTableHeader : public vector<thColumn>
{
};


// ============================================================================
// Fmt
// ============================================================================
namespace Fmt
{
	static thFormat Index		( wxALIGN_LEFT,	45 );
	static thFormat Name		( wxALIGN_LEFT,	200 );
	static thFormat Group		( wxALIGN_LEFT,	215 );
	static thFormat Flag		( wxALIGN_LEFT,	500 );
	static thFormat Offset		( wxALIGN_LEFT,	75 );
	static thFormat IndexOf		( wxALIGN_RIGHT,	45 );
	static thFormat Size		( wxALIGN_RIGHT,	75 );
}

#endif UNTABLEHEADER_H
// ============================================================================
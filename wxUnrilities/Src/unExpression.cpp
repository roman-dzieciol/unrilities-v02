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
//	unExpression.cpp
// ============================================================================
#include "prec.h"
#include "unExpression.h"


// ============================================================================
// unExpression
// ============================================================================


// ============================================================================
// unMathEx
// ============================================================================

unMathEx::unMathEx() : bIsValid(false), Pattern(wxEmptyString), PatternBase(10), PatternNumber(0) 
{
}

bool unMathEx::Compile( const wxString& pattern, int flags )
{
	bIsValid = CompileInternal( pattern, flags );
	return bIsValid;
}

bool unMathEx::CompileInternal( const wxString& pattern, int WXUNUSED(flags) )
{
	// remember patter
	Pattern = pattern;

	// remove white-space
	Pattern.Trim(false);
	Pattern.Trim(true);

	// empty is invalid
	if( Pattern.IsEmpty() )
		return false;

	bNegate = Pattern.Left(1).IsSameAs('!');
	if( bNegate )
		Pattern = Pattern.Mid(1);

	// empty is invalid
	if( Pattern.IsEmpty() )
		return false;

	// try base-10
	if( IsPatternValid( wxT("^[-+]?[0-9]+$"), Pattern, 10 ) )
		return true;

	// try base-16
	if( IsPatternValid( wxT("^[-+]?0x[0-9aA-fF]+$"), Pattern, 16 ) )
		return true;

	wxLogError( wxT("Invalid number '%s', use: '[!][+-]123' or '[!][+-]0xABC'"), Pattern.wx_str() );
	return false;
}


bool unMathEx::IsPatternValid( const wxString& expr, const wxString& pattern, int base )
{
	wxRegEx regex;

	if( regex.Compile( expr, wxRE_DEFAULT ) )
	{
		if( regex.Matches(pattern) )
		{
			wxChar* endptr;
			PatternNumber = wxStrtol( pattern.wx_str(), &endptr, base );
			PatternBase = base;
			//wxLogWarning( wxT("Base-10: %d"), PatternNumber );
			return true;
		}
		else
		{
			//wxLogError( wxT("Base-10: %d"), PatternNumber );
		}
	}
	else
	{
		wxLogError( wxT("Invalid RegEx %s, line: %d"), __FUNCTION__, __LINE__ );
	}

	return false;
}

bool unMathEx::IsValid() const
{
	return bIsValid;
}

bool unMathEx::Matches( const wxChar* text, int WXUNUSED(flags) ) const
{
	// ASSUME: text is valid base-10/16 number
	wxChar* endptr;
	return bNegate ^ (wxStrtol( text, &endptr, 0 ) == PatternNumber);
}

wxString unMathEx::GetMatch( const wxString& WXUNUSED(text), size_t WXUNUSED(index) ) const
{
	return wxEmptyString;
}


// ============================================================================
// unRegEx
// ============================================================================
bool unRegEx::Compile( const wxString& pattern, int flags )
{
	return wxRegEx::Compile( pattern, flags );
}

bool unRegEx::IsValid() const
{
	return wxRegEx::IsValid();
}

bool unRegEx::Matches( const wxChar* text, int flags ) const
{
	return wxRegEx::Matches( text, flags );
}

wxString unRegEx::GetMatch( const wxString& text, size_t index ) const
{
	return wxRegEx::GetMatch( text, index );
}



// ============================================================================
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
//	unExpression.h
// ============================================================================
#ifndef UNEXPRESSION_H
#define UNEXPRESSION_H


// ============================================================================
// unExpression
// ============================================================================
class unExpression
{
public:
	virtual bool Compile( const wxString& pattern, int flags = 0 ) = 0;
	virtual bool IsValid() const = 0;
	virtual bool Matches( const wxChar* text, int flags = 0 ) const = 0;
	virtual wxString GetMatch( const wxString& text, size_t index = 0 ) const = 0;
};


// ============================================================================
// unMathEx
// ============================================================================
struct unMathEx : public unExpression
{
	unMathEx();
	virtual bool Compile( const wxString& pattern, int flags = 0 );
	virtual bool IsValid() const;
	virtual bool Matches( const wxChar* text, int flags = 0 ) const;
	virtual wxString GetMatch( const wxString& text, size_t index = 0 ) const;

protected:
	bool CompileInternal( const wxString& pattern, int flags = 0 );
	bool IsPatternValid( const wxString& regex, const wxString& pattern, int base );

protected:
	bool		bIsValid;
	bool		bNegate;
	wxString	Pattern;
	int			PatternBase;
	long int	PatternNumber;
};


// ============================================================================
// unRegEx
// ============================================================================
struct unRegEx : public unExpression, public wxRegEx
{
	virtual bool Compile( const wxString& pattern, int flags = 0 );
	virtual bool IsValid() const;
	virtual bool Matches( const wxChar* text, int flags = 0 ) const;
	virtual wxString GetMatch( const wxString& text, size_t index = 0 ) const;
};


#endif UNEXPRESSION_H
// ============================================================================
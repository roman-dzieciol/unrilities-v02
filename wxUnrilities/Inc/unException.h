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
//	unException.h
// ============================================================================
#ifndef UNEXCEPTION_H
#define UNEXCEPTION_H


// ============================================================================
// unException
// ============================================================================
class unException
{

};


// ============================================================================
// exFail
// ============================================================================
class exFail : public unException, public exception
{	
public:
	explicit exFail( const char* message = "unknown" )
		: exception(message)
	{
	}
	explicit exFail( const wxChar* message)
		: exception(wxString(message).mb_str())
	{
	}
};


// ============================================================================
// exSafe
// ============================================================================
class exSafe : public unException
{
};


// ============================================================================
// SafePtr
// ============================================================================
template<class T> inline T* SafePtr(T* t)
{
	if( t != NULL )
		return t;
	throw exSafe();
}

// ============================================================================
// SafeTreeID
// ============================================================================
inline const wxTreeItemId& SafeTreeID( const wxTreeItemId& id )
{
	if( id.IsOk() )	
		return id;
	throw exSafe();
}


#endif UNEXCEPTION_H
// ============================================================================
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
//	unTypes.h
// ============================================================================
#ifndef UNTYPES_H
#define UNTYPES_H


// ============================================================================
// POD
// ============================================================================
typedef unsigned __int8		byte;
typedef unsigned __int16	word;
typedef unsigned __int32	dword;
typedef unsigned __int64	qword;

typedef signed __int8		int8;
typedef signed __int16		int16;
typedef signed __int32		int32;
typedef signed __int64		int64;

typedef const char*			cstr;


// ============================================================================
// ownvector
// ============================================================================
template <class T>
class ownvector : public vector<T*>
{
public:
    ~ownvector()
    {
        for( iterator it=begin(); it!=end(); ++it )
            delete static_cast<T*>(*it);
    }

    T* push_existing( T* p )
    {
        if( p != NULL )
            push_back(p);
        return p;
    }
};


#endif UNTYPES_H
// ============================================================================
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
//	unFlagManager.h
// ============================================================================
#ifndef UNFLAGMANAGER_H
#define UNFLAGMANAGER_H


typedef multimap<dword,wxString>		FlagMap;
typedef FlagMap::iterator				FlagMapItr;
typedef FlagMap::const_iterator			FlagMapCItr;
typedef pair<FlagMapItr,FlagMapItr>		FlagMapRng;

WX_DECLARE_STRING_HASH_MAP( FlagMap,	FlagData );
typedef FlagData::iterator				FlagDataItr;
typedef FlagData::const_iterator		FlagDataCItr;


// ============================================================================
// CachedRequest
// ============================================================================
template <class T>
struct CachedRequest
{
    wxString    Query;
    T           Found;
    bool        Result;
};


// ============================================================================
// unFlagManager
// ============================================================================
class unFlagManager
{
public:
    void LoadFlags();
    FlagDataCItr GetGroup( const wxString& query ) const;
    bool FindGroup( const wxString& query, FlagDataCItr& found ) const;
    void GetText( FlagDataCItr it, dword data, wxString& Text ) const;

protected:
	void NameUnknownBits( FlagMap& f, dword bits=32 );
	void SetFlag( FlagMap& f, const dword& d, const wxString& s );
	void Insert( FlagMap& flagmap, const wxString& mapname );


protected:
	FlagData    Data;	
    mutable CachedRequest<FlagDataCItr> CachedGroup;	
    mutable CachedRequest<FlagMapCItr>  CachedFlag;		
};


// ============================================================================
// global flags
// ============================================================================
class unGlobalFlag
{
public:
	static unFlagManager	Manager;
};


#define GFlag unGlobalFlag::Manager


// ============================================================================
// new
// ============================================================================

class unFlagItem
{
public:
    unFlagItem( dword data, const wxString& caption )
    :   Data(data)
    ,   Caption(caption)
    {
    }

    operator dword() const{ return Data; }

protected:
    dword Data;
    wxString Caption;
};

#define FLAG_GROUP_BEGIN(F) namespace F {
#define FLAG_GROUP_END() }
#define FLAG_DEFINE(D,T) static unFlagItem T( (D), (#T) );


FLAG_GROUP_BEGIN( PackageFlags )
    FLAG_DEFINE( 0x0001, AllowDownload )
    FLAG_DEFINE( 0x0002, ClientOptional )
    FLAG_DEFINE( 0x0004, ServerSideOnly )
    FLAG_DEFINE( 0x0008, BrokenLinks )
    FLAG_DEFINE( 0x0010, Unsecure )
    FLAG_DEFINE( 0x8000, Need )
FLAG_GROUP_END()

FLAG_GROUP_BEGIN( ObjectFlags )
    FLAG_DEFINE( 0x00000001, Transactional )
    FLAG_DEFINE( 0x00000002, Unreachable )
    FLAG_DEFINE( 0x00000004, Public )
    FLAG_DEFINE( 0x00000008, TagImp )
    FLAG_DEFINE( 0x00000010, TagExp )
    FLAG_DEFINE( 0x00000020, SourceModified )
    FLAG_DEFINE( 0x00000040, TagGarbage )
    FLAG_DEFINE( 0x00000200, NeedLoad )
    FLAG_DEFINE( 0x00000400, HighlightedName )
    FLAG_DEFINE( 0x00000400, EliminateObject )
    FLAG_DEFINE( 0x00000800, InSingularFunc )
    FLAG_DEFINE( 0x00000800, RemappedName )
    FLAG_DEFINE( 0x00001000, Suppress )
    FLAG_DEFINE( 0x00001000, StateChanged )
    FLAG_DEFINE( 0x00002000, InEndState )
    FLAG_DEFINE( 0x00004000, Transient )
    FLAG_DEFINE( 0x00008000, Preloading )
    FLAG_DEFINE( 0x00010000, LoadForClient )
    FLAG_DEFINE( 0x00020000, LoadForServer )
    FLAG_DEFINE( 0x00040000, LoadForEdit )
    FLAG_DEFINE( 0x00080000, Standalone )
    FLAG_DEFINE( 0x00100000, NotForClient )
    FLAG_DEFINE( 0x00200000, NotForServer )
    FLAG_DEFINE( 0x00400000, NotForEdit )
    FLAG_DEFINE( 0x00800000, Destroyed )
    FLAG_DEFINE( 0x01000000, NeedPostLoad )
    FLAG_DEFINE( 0x02000000, HasStack )
    FLAG_DEFINE( 0x04000000, Native )
    FLAG_DEFINE( 0x08000000, Marked )
    FLAG_DEFINE( 0x10000000, ErrorShutdown )
    FLAG_DEFINE( 0x20000000, DebugPostLoad )
    FLAG_DEFINE( 0x40000000, DebugSerialize )
    FLAG_DEFINE( 0x80000000, DebugDestroy )
FLAG_GROUP_END()


#endif UNFLAGMANAGER_H
// ============================================================================
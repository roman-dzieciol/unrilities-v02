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
//	unFlagManager.cpp
// ============================================================================
#include "prec.h"
#include "unFlagManager.h"

#define PKGFLAG()

FlagDataCItr unFlagManager::GetGroup( const wxString& query ) const
{
    FlagDataCItr found;
    if( FindGroup( query, found ) == false )
        FindGroup( wxT("Default"), found );
    return found;
}

bool unFlagManager::FindGroup( const wxString& query, FlagDataCItr& found ) const
{
    if( query == CachedGroup.Query )
    {
        found = CachedGroup.Found;
        return CachedGroup.Result;
    }

    found = Data.find(query); 
    CachedGroup.Query = query;
    CachedGroup.Found = found;    
    CachedGroup.Result = found != Data.end();
    return CachedGroup.Result;
}



void unFlagManager::GetText( FlagDataCItr it, dword data, wxString& Text ) const
{
	if( data > 0 )
	{
		const FlagMap& fm = (*it).second;
		dword bit, last	= 0;
		bool found	= false;

		for( FlagMapCItr fi=fm.begin(); fi!=fm.end(); ++fi )
		{
			bit = (*fi).first;
			if( bit & data )
			{
				if( found )
					Text << ((last == bit) ? "/" : ", ");
				last = bit;
				found = true;
				Text << (*fi).second;
			}
		}
	}
}

void unFlagManager::LoadFlags()
{
	// helper
	FlagMap tomap;

	// Default flagmap
	tomap.clear();
	NameUnknownBits(tomap);
	Insert(tomap,wxT("Default"));

	// PackageFlags
	tomap.clear();
	SetFlag( tomap, 0x0001, wxT("AllowDownload") );
	SetFlag( tomap, 0x0002, wxT("ClientOptional") );
	SetFlag( tomap, 0x0004, wxT("ServerSideOnly") );
	SetFlag( tomap, 0x0008, wxT("BrokenLinks") );
	SetFlag( tomap, 0x0010, wxT("Unsecure") );
	SetFlag( tomap, 0x8000, wxT("Need") );
	NameUnknownBits(tomap);
	Insert(tomap,wxT("PackageFlags"));

	// ObjectFlags
	tomap.clear();
	SetFlag( tomap, 0x00000001, wxT("Transactional") );
	SetFlag( tomap, 0x00000002, wxT("Unreachable") );
	SetFlag( tomap, 0x00000004, wxT("Public") );
	SetFlag( tomap, 0x00000008, wxT("TagImp") );
	SetFlag( tomap, 0x00000010, wxT("TagExp") );
	SetFlag( tomap, 0x00000020, wxT("SourceModified") );
	SetFlag( tomap, 0x00000040, wxT("TagGarbage") );
	SetFlag( tomap, 0x00000200, wxT("NeedLoad") );
	SetFlag( tomap, 0x00000400, wxT("HighlightedName") );
	SetFlag( tomap, 0x00000400, wxT("EliminateObject") );
	SetFlag( tomap, 0x00000800, wxT("InSingularFunc") );
	SetFlag( tomap, 0x00000800, wxT("RemappedName") );
	SetFlag( tomap, 0x00001000, wxT("Suppress") );
	SetFlag( tomap, 0x00001000, wxT("StateChanged") );
	SetFlag( tomap, 0x00002000, wxT("InEndState") );
	SetFlag( tomap, 0x00004000, wxT("Transient") );
	SetFlag( tomap, 0x00008000, wxT("Preloading") );
	SetFlag( tomap, 0x00010000, wxT("LoadForClient") );
	SetFlag( tomap, 0x00020000, wxT("LoadForServer") );
	SetFlag( tomap, 0x00040000, wxT("LoadForEdit") );
	SetFlag( tomap, 0x00080000, wxT("Standalone") );
	SetFlag( tomap, 0x00100000, wxT("NotForClient") );
	SetFlag( tomap, 0x00200000, wxT("NotForServer") );
	SetFlag( tomap, 0x00400000, wxT("NotForEdit") );
	SetFlag( tomap, 0x00800000, wxT("Destroyed") );
	SetFlag( tomap, 0x01000000, wxT("NeedPostLoad") );
	SetFlag( tomap, 0x02000000, wxT("HasStack") );
	SetFlag( tomap, 0x04000000, wxT("Native") );
	SetFlag( tomap, 0x08000000, wxT("Marked") );
	SetFlag( tomap, 0x10000000, wxT("ErrorShutdown") );
	SetFlag( tomap, 0x20000000, wxT("DebugPostLoad") );
	SetFlag( tomap, 0x40000000, wxT("DebugSerialize") );
	SetFlag( tomap, 0x80000000, wxT("DebugDestroy") );
	NameUnknownBits(tomap);
	Insert(tomap,wxT("ObjectFlags"));

}

void unFlagManager::NameUnknownBits( FlagMap& f, dword bits )
{
	wxString tostring;
	dword IdxBit = 1;
	for( dword i=1; i<=bits; ++i )
	{
		if( f.count(IdxBit) == 0 )
		{
			tostring << wxT("#") << i;
			SetFlag( f, IdxBit, tostring );
			tostring.clear();
		}
		IdxBit <<= 1;
	}
}

void unFlagManager::SetFlag( FlagMap& f, const dword& d, const wxString& s )
{
	f.insert( make_pair<dword,wxString>(d,s) );
}

void unFlagManager::Insert( FlagMap& flagmap, const wxString& mapname )
{
	Data[mapname] = flagmap;
}


unFlagManager unGlobalFlag::Manager = unFlagManager();

// ============================================================================
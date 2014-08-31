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
//	tpFormatter.h
// ============================================================================
#ifndef TPFORMATTER_H
#define TPFORMATTER_H

#include "tpTemplate.h"
#include "pkgSerializer.h"
#include "unFlagManager.h"


// ============================================================================
// PrintFormats
// ============================================================================
namespace PrintFormats
{
	static cstr Offset		= "0x%.8X";
	static cstr Index		= "%d";
	static cstr ObjProp		= "%s";
}

namespace pf = PrintFormats;


// ============================================================================
// Formatter :: tf_Stream
// ============================================================================
struct tf_Stream
{
	template <class T>
	void FormatText( wxString& text, T& data )
	{
		text << data;
	}
};


// ============================================================================
// Formatter :: tf_Printf
// ============================================================================
struct tf_Printf
{
	tf_Printf( cstr format ) : Format(format) {}

	template <class T>
	void FormatText( wxString& text, T& data )
	{
		text.Printf(Format, data);
	}

	template <>
	void FormatText( wxString& text, unIndex& data )
	{
		text.Printf(Format, data.GetData());
	}

	template <>
	void FormatText( wxString& text, wxString& data )
	{
		text.Printf(Format, data.wx_str());
	}

	cstr Format;
};


// ============================================================================
// Alias :: tf_Printf
// ============================================================================
#define tf_Offset() tf_Printf(PrintFormats::Offset)


// ============================================================================
// Formatter :: tf_Flags
// ============================================================================
struct tf_Flags
{
	tf_Flags( FlagDataCItr fit ) : FIt(fit) {}

	void FormatText( wxString& text, const dword& data )
	{
		GFlag.GetText(FIt,data,text);
	}

	FlagDataCItr FIt;
};


// ============================================================================
// Formatter :: tf_NameTable
// ============================================================================
struct tf_NameTable
{
	tf_NameTable( pkgSerializer& serializer ) : Serializer(serializer) {}

	template <class T>
	void FormatText( wxString& text, T& data )
	{
		text = Serializer.NameTable AT(data).Name;
	}

	pkgSerializer& Serializer;
};


// ============================================================================
// Aliases :: tf_NameTable
// ============================================================================
#define tf_Name() tf_NameTable(*Serializer)


// ============================================================================
// Formatter :: tf_ObjectRef
// ============================================================================
struct tf_ObjectRef
{
	tf_ObjectRef( pkgSerializer& serializer ) : Serializer(serializer) {}

	template <class T>
	void FormatText( wxString& text, T& data )
	{
		text = Serializer.GetObjectName(data);
	}

	pkgSerializer&	Serializer;
};


// ============================================================================
// Aliases :: tf_ObjectRef
// ============================================================================
#define tf_ObjRef() tf_ObjectRef(*Serializer)


#endif TPFORMATTER_H
// ============================================================================
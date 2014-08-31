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
//	tpComparer.h
// ============================================================================
#ifndef TPCOMPARER_H
#define TPCOMPARER_H

#include "tpTemplate.h"



// ============================================================================
// Comparer :: ic_Data
// ============================================================================
struct ic_Data
{
	template<class T>
	bool Compare( tdCell& itema, dword rowa, tdCell& itemb, dword rowb, T* reader )
	{
		return reader->DataSrc.GetData(itema,rowa) < reader->DataSrc.GetData(itemb,rowb);
	}
};


// ============================================================================
// Comparer :: ic_Text
// ============================================================================
struct ic_Text
{
	template<class T>
	bool Compare( tdCell& itema, dword rowa, tdCell& itemb, dword rowb, T* reader )
	{
		return reader->GetText(itema,rowa) < reader->GetText(itemb,rowb);
	}
};


// ============================================================================
// Comparer :: ic_TextNoCase
// ============================================================================
struct ic_TextNoCase
{
	template<class T>
	bool Compare( tdCell& itema, dword rowa, tdCell& itemb, dword rowb, T* printer )
	{
		return printer->Print(itema,rowa).compare(printer->Print(itemb,rowb)) < 0;
	}
};


#endif TPCOMPARER_H
// ============================================================================
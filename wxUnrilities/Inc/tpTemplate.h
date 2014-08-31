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
//	tpTemplate.h
// ============================================================================
#ifndef TPTEMPLATE_H
#define TPTEMPLATE_H

#include "unTableData.h"


// ============================================================================
// tpPrinter :: creates table cell text on demand
// ============================================================================
struct tpPrinter
{
	virtual wxString& Print( tdCell& item, dword WXUNUSED(row) )
	{
		if( item.Text.empty() )
		{
			item.Text = wxT("N/A");
		}
		return item.Text;
	}

	virtual bool Compare( tdCell& itema, dword WXUNUSED(rowa), tdCell& itemb, dword WXUNUSED(rowb) )
	{
		return itema.Data < itemb.Data;
	}
};


// ============================================================================
// Item :: tpiTemplate
// ============================================================================
// TF = Text Formatter
// DS = Data Source
// IC = Item Compare
// ============================================================================
template <class TF, class DS, class IC>
class tpiTemplate : public tpPrinter
{
public:
	explicit tpiTemplate
		( const TF&	textfmt = TF()
		, const DS&	datasrc = DS()
		, const IC&	itemcmp = IC())  
		: TextFmt(textfmt), DataSrc(datasrc), ItemCmp(itemcmp) 
	{
	}

	virtual wxString& Print( tdCell& item, dword row )
	{
		if( item.Text.empty() )
		{
			TextFmt.FormatText( item.Text, DataSrc.GetData(item,row) );
		}
		return item.Text;
	}

	virtual bool Compare( tdCell& itema, dword rowa, tdCell& itemb, dword rowb )
	{
		return ItemCmp.Compare(itema,rowa,itemb,rowb,this);
	}

public:
	TF	TextFmt;
	DS	DataSrc;
	IC	ItemCmp;
};


// ============================================================================
// Creator :: tpiTemplate
// ============================================================================
template< class TF, class DS, class IC >
tpiTemplate<TF,DS,IC>* new_printer( const TF& tf, const DS& ds, const IC& ic )
{ 
	return new tpiTemplate<TF,DS,IC>(tf,ds,ic);	// delete at ~unTablePrinter 
}


#endif TPTEMPLATE_H
// ============================================================================
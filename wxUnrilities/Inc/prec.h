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
//	prec.h 
// ============================================================================
#ifndef PREC_H
#define PREC_H


//
// wxWidgets
//

#pragma warning( push )
#pragma warning( disable: 4702 )	// unreachable code
#pragma warning( disable: 4996 )	// deprecated
#pragma warning( disable: 4267 )	// conversion, possible loss of data

#include <wx/wxprec.h>
#include <wx/utils.h>
#include <wx/regex.h>
#include <wx/datetime.h>

#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>

#include <wx/file.h>
#include <wx/ffile.h>
#include <wx/textfile.h>

//
// STL
//

#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <list>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

#pragma warning( pop )


//
// Unrilities macros
//

// namespace
#define NAMESPACE_BEGIN(N)	namespace N {
#define NAMESPACE_END		}
#define NS_UNRILITIES			unrilities


// member function/variable access
#define MEMBER_REF(Object,MemberPtr)  ((Object).*(MemberPtr))
#define MEMBER_PTR(Object,MemberPtr)  ((Object)->*(MemberPtr))


// adds file name and line number to memory leak reports
#ifdef _DEBUG
    #ifdef __WXMSW__
	    #undef WXDEBUG_NEW
	    #define WXDEBUG_NEW new(__FILE__,__LINE__)
	    #include <crtdbg.h>

	    // this define works around a bug with inline declarations of new, see
	    // http://support.microsoft.com/support/kb/articles/Q140/8/58.asp
        #define new  new( _NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#endif


// vector accessor macro
#ifdef _DEBUG
	#define AT(num)	.at(num)
#else
	#define AT(num)	[num]
#endif


//
// Unrilities headers
//

#pragma warning( disable: 4511 )	// copy constructor could not be generated
#pragma warning( disable: 4512 )	// assignment operator could not be generated

#include "unTypes.h"
#include "unException.h"
#include "unTime.h"



#endif PREC_H
// ============================================================================
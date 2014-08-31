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
//	unLogToStatus.cpp
// ============================================================================
#include "prec.h"
#include "unLogToStatus.h"

unLogToStatus::unLogToStatus()
{
	lognew = new wxLogGui;
	logold = wxLog::SetActiveTarget(this);
	logold->Suspend();
}

unLogToStatus::~unLogToStatus()
{
	delete lognew;
	logold->Resume();
	wxLog::SetActiveTarget(logold);
}

void unLogToStatus::Flush()
{
}

void unLogToStatus::DoLog( wxLogLevel level, const wxChar* szString, time_t t )
{
	switch( level ) 
	{
	case wxLOG_Trace:
		break;

	case wxLOG_Error:
		level = wxLOG_Status;

	default:
		// bogus cast just to access protected DoLog
		if( lognew  )
			static_cast<unLogToStatus *>(lognew)->DoLog(level, szString, t);		
	}
}

void unLogToStatus::DoLogString( const wxChar* WXUNUSED(szString), time_t WXUNUSED(t) )
{
}


// ============================================================================
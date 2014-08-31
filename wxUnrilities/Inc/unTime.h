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
//	unTime.h :: __WXMSW__ wxStopWatch replacement
// ============================================================================
#ifndef UNTIME_H
#define UNTIME_H

#ifdef __WXMSW__

class unTime
{
public:
	typedef float	tmult;
	typedef float	ttime;

	unTime()
	{
		if( Frequency == 0 )
		{
			Init();
		}
		Start();
	}

	void Init()
	{
		CalcFrequency();
		CalcMultiplier();
	}

	void Start()
	{
		StartTicks = 0;
		SavedTicks = 0;
		GetTicks(StartTicks);
	}

	ttime Time()
	{
		qword total = SavedTicks;
		if( StartTicks != 0 )
		{
			qword counter;
			GetTicks(counter);
			total += counter-StartTicks;
		}
		return static_cast<ttime>(total * Multiplier);
	}

	void Pause()
	{
		if( StartTicks != 0 )
		{
			qword counter;
			GetTicks(counter);
			SavedTicks += counter - StartTicks;
			StartTicks = 0;
		}
		else
		{
			//wxLogMessage( wxT("%s :: timer already paused!"), __FUNCTION__ );
		}
	}

	void Resume()
	{
		if( StartTicks == 0 )
		{
			GetTicks(StartTicks);
		}
		else
		{
			//wxLogMessage( wxT("%s :: timer already resumed!"), __FUNCTION__ );
		}
	}


protected:
	void CalcFrequency()
	{
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&Frequency));
	}

	void CalcMultiplier()
	{
		Multiplier = static_cast<tmult>( Unit / static_cast<double>(Frequency)  );
	}

	void GetTicks( qword& counter )
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
	}


protected:
	qword	StartTicks;				// Counter ticks at last Start() or Resume() 
	qword	SavedTicks;				// Total ticks stored by Pause() 
	static	tmult	Multiplier;		// Multiply ticks by this to get time in Unit's
	static	qword	Frequency;		// Clock frequency
	static	double	Unit;			// Time unit, 1 = s, 1000 = ms
};

#define wxStopWatch			unTime
#endif // __WXMSW__


#define TimeStr(sw)			wxString::Format(wxT("%.2fms"),sw.Time()).wx_str()

#define	SW_START(sw)		sw.Start()	
#define	SW_PAUSE(sw)		sw.Pause()	

#define SW_DEF(sw)			wxStopWatch sw;
#define SW_LOGF(sw)			sw.Pause(); wxLogMessage(("%s %s"), __FUNCTION__, TimeStr(sw));
#define SW_LOG(sw,txt)		sw.Pause(); wxLogMessage((txt), TimeStr(sw));
#define SW_LOGV(sw,txt,wtf)	sw.Pause(); wxLogMessage((txt), TimeStr(sw), wtf);
#define SW_VLOG(sw,txt,wtf)	sw.Pause(); wxLogMessage((txt), wtf, TimeStr(sw) );


#endif UNTIME_H
// ============================================================================
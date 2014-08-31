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
//	unLogWindow.h
// ============================================================================
#ifndef UNLOGWINDOW_H
#define UNLOGWINDOW_H

class unLogWindow;


// ============================================================================
// unLogFrame
// ============================================================================
class unLogFrame : public wxFrame
{
public:
	unLogFrame( wxWindow* parent, unLogWindow* log, const wxChar* caption );
	virtual ~unLogFrame();

public:
	wxTextCtrl *TextCtrl() const { return Text; }

protected:
	virtual void DoClose();
	virtual void SaveLog();
	void OnCloseWindow(wxCloseEvent& event);

protected:
	wxTextCtrl*		Text;
	unLogWindow*	Log;

private:
	DECLARE_EVENT_TABLE()
};


// ============================================================================
// unLogWindow
// ============================================================================
class unLogWindow : public wxLogPassThrough
{
public:
	unLogWindow( wxWindow* parent,	const wxChar* caption, bool bpassmsg = false );
	virtual ~unLogWindow();

public:
	virtual void Show( bool bshow = true );
	virtual wxFrame* GetFrame() const;

protected:
    virtual void DoLogTextAtLevel( wxLogLevel level, const wxChar *str );
    virtual void DoLogText( const wxChar *str );

protected:
    unLogFrame* LogFrame; 
};


#endif UNLOGWINDOW_H
// ============================================================================
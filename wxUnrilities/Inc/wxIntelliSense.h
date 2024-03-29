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
//	wxIntelliSense.h :: intellisense helper, do not include.
// ============================================================================

#define WXDLLEXPORT
#define WXEXPORT
#define wxUSE_GUI 1
#define wxUSE_MSGDLG 1
#define __WXMSW__ 1

#undef wxWindow
class wxWindow : public wxWindowMSW
{
};

#include "wx/confbase.h"
#include "wx/config.h"

#include "wx/gdicmn.h"
#include "wx/datetime.h"

//#include "wx/msw/accel.h"
#include "wx/app.h"
#include "wx/msw/app.h"
#include "wx/msw/bitmap.h"
//#include "wx/msw/bmpbuttn.h"
//#include "wx/msw/brush.h"
#include "wx/button.h"
#include "wx/msw/button.h"
#include "wx/msw/caret.h"
#include "wx/ctrlsub.h"
#include "wx/checkbox.h"
#include "wx/msw/checkbox.h"
#include "wx/msw/checklst.h"
#include "wx/choice.h"
#include "wx/msw/choice.h"
//#include "wx/msw/clipbrd.h"
//#include "wx/msw/colordlg.h"
#include "wx/msw/colour.h"
#include "wx/combobox.h"
#include "wx/msw/combobox.h"
#include "wx/control.h"
#include "wx/msw/control.h"
//#include "wx/msw/ctl3d.h"
//#include "wx/msw/cursor.h"
#include "wx/dc.h"
#include "wx/msw/dc.h"
#include "wx/dcclient.h"
#include "wx/msw/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/msw/dcmemory.h"
//#include "wx/msw/dcprint.h"
//#include "wx/msw/dcscreen.h"
#include "wx/msw/dde.h"
#include "wx/msw/dialog.h"
#include "wx/msw/dib.h"
#include "wx/msw/dirdlg.h"
//#include "wx/msw/display.h"
#include "wx/msw/dragimag.h"
#include "wx/msw/enhmeta.h"
//#include "wx/msw/fdrepdlg.h"
#include "wx/msw/filedlg.h"
#include "wx/confbase.h"
#include "wx/fileconf.h"
//#include "wx/msw/font.h"
//#include "wx/msw/fontdlg.h"
//#include "wx/msw/fontenum.h"
#include "wx/toplevel.h"
#include "wx/msw/toplevel.h"

class wxTopLevelWindowNative : public wxTopLevelWindowMSW
{
};

class wxTopLevelWindow : public wxTopLevelWindowMSW
{
};

#include "wx/frame.h"
#include "wx/msw/frame.h"

//#include "wx/msw/gauge95.h"
//#include "wx/msw/gccpriv.h"
//#include "wx/msw/gdiimage.h"
//#include "wx/msw/gdiobj.h"
//#include "wx/msw/glcanvas.h"
//#include "wx/msw/extra.h"
//#include "wx/msw/gl.h"
//#include "wx/msw/glaux.h"
//#include "wx/msw/glu.h"
//#include "wx/msw/sql.h"
//#include "wx/msw/sqlext.h"
//#include "wx/msw/sqltypes.h"
//#include "wx/msw/winresrc.h"
//#include "wx/msw/gsockmsw.h"
//#include "wx/msw/helpbest.h"
//#include "wx/msw/helpchm.h"
//#include "wx/msw/helpwin.h"
#include "wx/msw/icon.h"
#include "wx/msw/imaglist.h"
#include "wx/msw/iniconf.h"
#include "wx/msw/joystick.h"
#include "wx/msw/listbox.h"
#include "wx/msw/listctrl.h"
#include "wx/msw/mdi.h"
#include "wx/menu.h"
#include "wx/msw/menu.h"
#include "wx/msw/menuitem.h"
#include "wx/msw/metafile.h"
#include "wx/msw/microwin.h"
#include "wx/msw/mimetype.h"
#include "wx/minifram.h"
#include "wx/msw/minifram.h"
#include "wx/msw/missing.h"

#include "wx/msgdlg.h"
#include "wx/msw/msgdlg.h"

#include "wx/msw/mslu.h"
#include "wx/msw/msvcrt.h"
#include "wx/msw/notebook.h"
#include "wx/msw/access.h"
#include "wx/msw/automtn.h"
#include "wx/msw/dataform.h"
#include "wx/msw/dataobj.h"
#include "wx/msw/dataobj2.h"
#include "wx/msw/dropsrc.h"
#include "wx/msw/droptgt.h"
#include "wx/msw/oleutils.h"
#include "wx/msw/uuid.h"
#include "wx/msw/palette.h"
#include "wx/msw/pen.h"
#include "wx/msw/popupwin.h"
//#include "wx/msw/printdlg.h"
//#include "wx/msw/printwin.h"
#include "wx/msw/private.h"
#include "wx/msw/radiobox.h"
#include "wx/msw/radiobut.h"
#include "wx/msw/regconf.h"
#include "wx/msw/region.h"
#include "wx/msw/registry.h"
#include "wx/generic/scrolwin.h"
#include "wx/scrolwin.h"
class wxScrolledWindow : public wxGenericScrolledWindow
{
};

#include "wx/msw/scrolbar.h"
#include "wx/msw/setup.h"
//#include "wx/msw/setup0.h"
//#include "wx/msw/setup_microwin.h"
#include "wx/msw/slider95.h"
#include "wx/msw/slidrmsw.h"
#include "wx/msw/spinbutt.h"
#include "wx/msw/spinctrl.h"
#include "wx/msw/statbmp.h"
#include "wx/msw/statbox.h"
#include "wx/msw/statbr95.h"
#include "wx/msw/statline.h"
#include "wx/stattext.h"
#include "wx/msw/stattext.h"
#include "wx/msw/tabctrl.h"
#include "wx/msw/taskbar.h"
#include "wx/msw/tbar95.h"
#include "wx/msw/tbarmsw.h"
#include "wx/msw/textctrl.h"
#include "wx/msw/tglbtn.h"
#include "wx/timer.h"
#include "wx/msw/timer.h"
#include "wx/msw/tooltip.h"
#include "wx/msw/treectrl.h"
#include "wx/msw/wave.h"
#include "wx/msw/gdiobj.h"
#include "wx/msw/missing.h"
#include "wx/msw/setup.h"
#include "wx/window.h"
#include "wx/msw/window.h"
#include "wx/msw/winundef.h"
#include "wx/sizer.h"


const wxSize wxDefaultSize(wxDefaultCoord, wxDefaultCoord);
const wxPoint wxDefaultPosition(wxDefaultCoord, wxDefaultCoord);



// ============================================================================
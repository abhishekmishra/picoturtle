/*
 * picoturtle: PicoTurtleWin.h
 * Created on: 19-Feb-2019
 * 
 * picoturtle
 * Copyright (C) 2018 Abhishek Mishra <abhishekmishra3@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_PICOTURTLEWIN_H_
#define SRC_PICOTURTLEWIN_H_

#include <stdint.h>

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <string>

using namespace std;

class PicoTurtleCommand {
public:
	PicoTurtleCommand();
	virtual void exec(wxDC& dc);
	virtual ~PicoTurtleCommand();

	string name;
	uint64_t int1;
	uint64_t int2;
	uint64_t int3;
	uint64_t int4;
	double double1;
	double double2;
	double double3;
	double double4;
};

class PicoTurtleWin {
public:
	PicoTurtleWin();
	virtual ~PicoTurtleWin();

	void forward(uint64_t distance);

	void runCommands(wxDC& dc);

	void loadCommands();

	vector<PicoTurtleCommand*> commands;
};


class PicoTurtleDrawPane : public wxPanel
{

public:
    PicoTurtleDrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(PicoTurtleDrawPane, wxPanel)
// some useful events
/*
 EVT_MOTION(BasicDrawPane::mouseMoved)
 EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
 EVT_LEFT_UP(BasicDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(PicoTurtleDrawPane::paintEvent)

END_EVENT_TABLE()

class PicoTurtleApp: public wxApp {
public:
	virtual bool OnInit();
};

class PicoTurtleFrame: public wxFrame {
public:
	PicoTurtleFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};

enum {
	ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(PicoTurtleFrame, wxFrame)
EVT_MENU(ID_Hello, PicoTurtleFrame::OnHello)
EVT_MENU(wxID_EXIT, PicoTurtleFrame::OnExit)
EVT_MENU(wxID_ABOUT, PicoTurtleFrame::OnAbout)
wxEND_EVENT_TABLE()

#endif /* SRC_PICOTURTLEWIN_H_ */

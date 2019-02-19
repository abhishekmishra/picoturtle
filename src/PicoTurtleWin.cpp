/*
 * picoturtle: PicoTurtleWin.cpp
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

#include "PicoTurtleWin.h"
#include <iostream>

PicoTurtleWin* ptw = new PicoTurtleWin();

PicoTurtleCommand::PicoTurtleCommand() {
	//do nothing
}

PicoTurtleCommand::~PicoTurtleCommand() {
	//do nothing
}

void PicoTurtleCommand::exec(wxDC& dc) {
	if (name.compare("fd") == 0) {
		dc.DrawLine(300, 100, 700, 300);
	}
}

PicoTurtleWin::PicoTurtleWin() {
	loadCommands();
}

PicoTurtleWin::~PicoTurtleWin() {
	// TODO Auto-generated destructor stub
}

void PicoTurtleWin::runCommands(wxDC& dc) {

//    // draw some text
//    dc.DrawText(wxT("Testing"), 40, 60);
//
//    // draw a circle
//    dc.SetBrush(*wxGREEN_BRUSH); // green filling
//    dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
//    dc.DrawCircle( wxPoint(200,100), 25 /* radius */ );
//
//    // draw a rectangle
//    dc.SetBrush(*wxBLUE_BRUSH); // blue filling
//    dc.SetPen( wxPen( wxColor(255,175,175), 10 ) ); // 10-pixels-thick pink outline
//    dc.DrawRectangle( 300, 100, 400, 200 );
//
//    // draw a line
//    dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick
//    dc.DrawLine( 300, 100, 700, 300 ); // draw line across the rectangle
//
//    // Look at the wxDC docs to learn how to draw other stuff

	cout << "running commands" << endl;
	for (auto i = commands.begin(); i != commands.end(); ++i) {
		PicoTurtleCommand* cmd = *i;
		cmd->exec(dc);
		cout << cmd->name << " ";
	}
	cout << endl;
}

void PicoTurtleWin::loadCommands() {
	PicoTurtleCommand* test_command = new PicoTurtleCommand();
	test_command->int1 = 100;
	test_command->name = "fd";
	commands.push_back(test_command);
}

// some useful events
/*
 void BasicDrawPane::mouseMoved(wxMouseEvent& event) {}
 void BasicDrawPane::mouseDown(wxMouseEvent& event) {}
 void BasicDrawPane::mouseWheelMoved(wxMouseEvent& event) {}
 void BasicDrawPane::mouseReleased(wxMouseEvent& event) {}
 void BasicDrawPane::rightClick(wxMouseEvent& event) {}
 void BasicDrawPane::mouseLeftWindow(wxMouseEvent& event) {}
 void BasicDrawPane::keyPressed(wxKeyEvent& event) {}
 void BasicDrawPane::keyReleased(wxKeyEvent& event) {}
 */

PicoTurtleDrawPane::PicoTurtleDrawPane(wxFrame* parent) :
		wxPanel(parent) {
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void PicoTurtleDrawPane::paintEvent(wxPaintEvent & evt) {
	wxPaintDC dc(this);
//	render(dc);
	ptw->runCommands(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 *
 * In most cases, this will not be needed at all; simply handling
 * paint events and calling Refresh() when a refresh is needed
 * will do the job.
 */
void PicoTurtleDrawPane::paintNow() {
	wxClientDC dc(this);
//    render(dc);
	ptw->runCommands(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void PicoTurtleDrawPane::render(wxDC& dc) {
	// draw some text
	dc.DrawText(wxT("Testing"), 40, 60);

	// draw a circle
	dc.SetBrush(*wxGREEN_BRUSH); // green filling
	dc.SetPen(wxPen( wxColor(255, 0, 0), 5)); // 5-pixels-thick red outline
	dc.DrawCircle(wxPoint(200, 100), 25 /* radius */);

	// draw a rectangle
	dc.SetBrush(*wxBLUE_BRUSH); // blue filling
	dc.SetPen(wxPen( wxColor(255, 175, 175), 10)); // 10-pixels-thick pink outline
	dc.DrawRectangle(300, 100, 400, 200);

	// draw a line
	dc.SetPen(wxPen( wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
	dc.DrawLine(300, 100, 700, 300); // draw line across the rectangle

	// Look at the wxDC docs to learn how to draw other stuff
}

wxIMPLEMENT_APP(PicoTurtleApp);

bool PicoTurtleApp::OnInit() {
	PicoTurtleFrame *frame = new PicoTurtleFrame("Hello World", wxPoint(50, 50),
			wxSize(450, 340));
	frame->Show(true);
	return true;
}

PicoTurtleFrame::PicoTurtleFrame(const wxString& title, const wxPoint& pos,
		const wxSize& size) :
		wxFrame(NULL, wxID_ANY, title, pos, size) {
	wxMenu *menuFile = new wxMenu;
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	PicoTurtleDrawPane* drawPane = new PicoTurtleDrawPane(this);
	sizer->Add(drawPane, 1, wxEXPAND);

	SetSizer(sizer);
	SetAutoLayout(true);

	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
			"Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
}

void PicoTurtleFrame::OnExit(wxCommandEvent& event) {
	Close(true);
}

void PicoTurtleFrame::OnAbout(wxCommandEvent& event) {
	wxMessageBox("This is a wxWidgets' Hello world sample", "About Hello World",
	wxOK | wxICON_INFORMATION);
}

void PicoTurtleFrame::OnHello(wxCommandEvent& event) {
	wxLogMessage
	("Hello world from wxWidgets!");
}

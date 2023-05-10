#pragma once

#include "wx/wx.h"
#include <string>

#define defaultWidth 5
#define defaultHeight 5 
#define defMines 5

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
	
public:
	int time=0;
	wxTimer m_timer;
	int nFieldWidth = defaultWidth;
	int nFieldHeight = defaultHeight;
	wxButton **btn; //array of events(pointers)
	int* nField = nullptr;
	bool bFirstClick = true; //that first click doesnt explode
	int* mineloc = nullptr;
	int minecount = 0;
	std::string name="Unnamed";
	int defaultMines=defMines;
	
	wxPanel *panel;
	wxStaticText *timerText;
	
	
	void OnButtonClicked(wxCommandEvent &evt);  //what to do if clicked
	void rclick(wxMouseEvent &evt);
	void OnTimer(wxTimerEvent &event);
	wxDECLARE_EVENT_TABLE();  //to record clicks
};

#pragma once

#include "wx/wx.h"

#define defaultWidth 4  
#define defaultHeight 3  

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
	
	void OnButtonClicked(wxCommandEvent &evt);  //what to do if clicked
	void rclick(wxMouseEvent &evt);
	void OnTimer(wxTimerEvent &event);
	wxDECLARE_EVENT_TABLE();  //to record clicks
};

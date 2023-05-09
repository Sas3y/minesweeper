#include "cApp.h"


wxIMPLEMENT_APP(cApp);

cApp::cApp()
{
}

cApp::~cApp()
{
}

bool cApp::OnInit()
{
	
	frame = new cMain();
	frame -> Show();
	return true;
	
	/*
	cMain* frame = new cMain("MS2");
	frame->SetClientSize(800,600);
	//frame->Center();
	frame->Show();
	return true;
	*/
}

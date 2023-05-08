#include "cMain.h"

#define defaultMines 1


wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "MS1", wxPoint(0,0), wxSize(1920,1080))
{
	btn = new wxButton*[cMain::nFieldWidth * cMain::nFieldHeight];
	
	wxGridSizer *grid = new wxGridSizer(cMain::nFieldWidth+1, cMain::nFieldHeight, 0, 0);  //generates a sizer which corrects positions of buttons
	
	
	/*
	wxTimer *timer = new wxTimer(this, wxID_ANY);
	timer->Start(1000);
	*/

	//CHATGPT - top panel for time and mine count
	/*
	wxPanel* topPanel = new wxPanel(this);
	wxBoxSizer* topPanelSizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText* mineCountLabel = new wxStaticText(topPanel, wxID_ANY, "Mine Count:");
	wxStaticText* timerLabel = new wxStaticText(topPanel, wxID_ANY, "Timer:");
	
	topPanelSizer->Add(mineCountLabel, 0, wxALL, 5);
	topPanelSizer->Add(timerLabel, 0, wxALL, 5);
	topPanel->SetSizer(topPanelSizer);
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	topSizer->Add(topPanel, 0, wxEXPAND);
	
	grid->Add(topSizer, 0, wxEXPAND);
	for (int i = 0; i < cMain::nFieldWidth * cMain::nFieldHeight; i++) {
    	grid->Add(new wxButton(this, wxID_ANY, ""), 0, wxEXPAND);
	}
	*/
	
	cMain::nField = new int[cMain::nFieldWidth * cMain:: nFieldHeight];  //board
	cMain::mineloc = new int[cMain::nFieldWidth * cMain:: nFieldHeight];  //mine positions
	
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	
	
	
	for(int x=0;x<cMain::nFieldWidth;x++)
	{
		for(int y=0;y<cMain::nFieldHeight;y++)
		{
			btn[y * (cMain::nFieldWidth) + x] = new wxButton(this, 10000 + (y * (cMain::nFieldWidth) + x));  //10000 is id
			
			btn[y * (cMain::nFieldWidth) + x]->SetFont(font);
			
			grid -> Add(btn[y * (cMain::nFieldWidth) + x], 1, wxEXPAND | wxALL);  //adjusts button size
			btn[y * cMain::nFieldWidth + x] -> Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			btn[y * cMain::nFieldWidth + x] -> Bind(wxEVT_RIGHT_DOWN, &cMain::rclick, this);
			cMain::nField[y * cMain::nFieldWidth + x] = 0;
			cMain::mineloc[y * cMain::nFieldWidth + x] = 0;
		}
	}
	
	//timer
	m_timer.SetOwner(this);
	m_timer.Start(1000);
	Bind(wxEVT_TIMER, &cMain::OnTimer, this, m_timer.GetId());
	
	//extra buttons
	btn[12] = new wxButton(this, 9990);
	grid-> Add(btn[12], 1, wxEXPAND | wxALL);
	btn[13] = new wxButton(this, 9991);
	grid-> Add(btn[13], 1, wxEXPAND | wxALL);
	btn[14] = new wxButton(this, 9992);
	grid-> Add(btn[14], 1, wxEXPAND | wxALL);
	
	this -> SetSizer(grid);
	grid -> Layout();
	
}


cMain::~cMain()
{
	delete[]btn;
	delete[]nField;
	delete[]mineloc;
}

void cMain::OnButtonClicked(wxCommandEvent &evt)
{
	int x = (evt.GetId() - 10000) % cMain::nFieldWidth;
	int y = (evt.GetId() - 10000) / cMain::nFieldWidth;
	
	if(cMain::bFirstClick)
	{
		int mines = defaultMines;
		cMain::minecount=mines;
		
		while (mines)
		{
			int rx = rand() % cMain::nFieldWidth;
			int ry = rand() % cMain::nFieldHeight;
			
			if(cMain::nField[ry * cMain::nFieldWidth + rx] == 0 && rx != x && ry != y)
			{
				cMain::nField[ry * cMain::nFieldWidth + rx] = -1;
				mines--;
			}
		}
		cMain::bFirstClick = false;
	}
	
	
	
	if(cMain::nField[y * cMain::nFieldWidth + x] == -1 && cMain::mineloc[y * cMain::nFieldWidth + x] != 1)
	{
		wxMessageBox("There was a mine");
		cMain::bFirstClick = true;  //reset
		
		for(int x=0;x<cMain::nFieldWidth;x++)
		{
			for(int y=0;y<cMain::nFieldHeight;y++)
			{
				cMain::nField[y * cMain::nFieldWidth + x] = 0;
				btn[y * cMain::nFieldWidth + x] -> SetLabel("");  //how many mines
				btn[y * cMain::nFieldWidth + x] -> Enable(true);
				cMain::mineloc[y * cMain::nFieldWidth + x] = 0;
			}
		}
	}
	else
	{
		//counts neighboring mines
		int mine_count = 0;
		
		for(int i=-1; i<2; i++)
		{
			for(int j = -1; j<2; j++)
			{
				if(x + i >= 0 && x + i < cMain::nFieldWidth && y + j >=0 && y + j < cMain::nFieldHeight)
				{
					if(cMain::nField[(y + j) * cMain::nFieldWidth + (x + i)] == -1)
					{
						mine_count++;
						
					}
				}
			}
		}
		
		if(cMain::mineloc[y * cMain::nFieldWidth + x] != 1)
		{
			btn[y * cMain::nFieldWidth + x] -> SetLabel(std::to_string(mine_count));
			
			btn[y*cMain::nFieldWidth + x] -> Enable(false);
		}

	}
	
	if(minecount==0)
	{
		wxMessageBox("You Won!");
		cMain::bFirstClick = true;
		
		for(int x=0;x<cMain::nFieldWidth;x++)
		{
			for(int y=0;y<cMain::nFieldHeight;y++)
			{
				cMain::nField[y * cMain::nFieldWidth + x] = 0;
				btn[y * cMain::nFieldWidth + x] -> SetLabel("");
				btn[y * cMain::nFieldWidth + x] -> Enable(true);
				cMain::mineloc[y * cMain::nFieldWidth + x] = 0;
			}
		}
	}
	
	
	
	evt.Skip();

}

void cMain::rclick(wxMouseEvent &evt)	//flag a mine
{	
	int x = (evt.GetId() - 10000) % cMain::nFieldWidth;
	int y = (evt.GetId() - 10000) / cMain::nFieldWidth;
	
	if(cMain::mineloc[y * cMain::nFieldWidth + x] == 0)
	{
		cMain::mineloc[y * cMain::nFieldWidth + x] = 1;
		btn[y * cMain::nFieldWidth + x] -> SetLabel("M");
		//btn[y * cMain::nFieldWidth + x] -> Enable(false);
	}
	else
	{
		cMain::mineloc[y * cMain::nFieldWidth + x] = 0;
		btn[y * cMain::nFieldWidth + x] -> SetLabel("");
		//btn[y * cMain::nFieldWidth + x] -> Enable(true);
	}
	if(cMain::mineloc[y * cMain::nFieldWidth + x] == 1 && cMain::nField[y * cMain::nFieldWidth + x] == -1)
	{
		cMain::minecount--;
	}
	evt.Skip();
}

void cMain::OnTimer(wxTimerEvent &event)
{
	time++;
	btn[12]->SetLabel(std::to_string(time)+" s");
}

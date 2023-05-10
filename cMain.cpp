#include "cMain.h"
#include <string>
#include <fstream>

//dont confuse minecount with mine_count

//gameplay bug: set minecount to 0 and press any (even flagged) tile to win
//reset time for highscores

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "MS1", wxPoint(0,0), wxSize(800,600))
{
	//maybe will pass for input?
	std::cout<<"Please provide your name:\n";
	std::cin>>cMain::name;
	std::cout<<"Please provide the dimensions you want to play with:\n";
	std::cin>>nFieldWidth>>nFieldHeight;
	std::cout<<"Please provide the amount of mines you want to play with:\n";
	std::cin>>cMain::defaultMines;
	
	//Options
	//NEEDS MORE:
	//HIGHSCORES, 
	wxMenuBar *menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu();
	menuBar->Append(fileMenu, _("Options"));
	SetMenuBar(menuBar);
	//wxMenuItem *restartBtn = new wxMenuItem(fileMenu, wx
	fileMenu->Append(wxID_ANY, _("&New game\tCtrl+R"));
	fileMenu->Append(wxID_ANY, _("&Highscores\tCtrl+H"));

	//timer
	m_timer.SetOwner(this);
	Bind(wxEVT_TIMER, &cMain::OnTimer, this, m_timer.GetId());
	wxStatusBar *statusBar = CreateStatusBar(2); //2 = size
	
	//minecounter init
	SetStatusText(_("Mines left: "+std::to_string(cMain::defaultMines)), 1);
	
	
	//creates button grid
	btn = new wxButton*[cMain::nFieldWidth * cMain::nFieldHeight]; 
	
	wxGridSizer *grid = new wxGridSizer(cMain::nFieldWidth, cMain::nFieldHeight, 0, 0);  //generates a sizer which corrects positions of buttons
	
	
	cMain::nField = new int[cMain::nFieldWidth * cMain:: nFieldHeight];  //board
	cMain::mineloc = new int[cMain::nFieldWidth * cMain:: nFieldHeight];  //flag positions
	
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	
	
	for(int x=0;x<cMain::nFieldWidth;x++)
	{
		for(int y=0;y<cMain::nFieldHeight;y++)
		{
			btn[y * (cMain::nFieldWidth) + x] = new wxButton(this, 10000 + (y * (cMain::nFieldWidth) + x));  //10000 is magic id
			
			btn[y * (cMain::nFieldWidth) + x]->SetFont(font);
			
			grid -> Add(btn[y * (cMain::nFieldWidth) + x], 1, wxEXPAND | wxALL);  //adjusts button size
			btn[y * cMain::nFieldWidth + x] -> Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			btn[y * cMain::nFieldWidth + x] -> Bind(wxEVT_RIGHT_DOWN, &cMain::rclick, this);
			cMain::nField[y * cMain::nFieldWidth + x] = 0;
			cMain::mineloc[y * cMain::nFieldWidth + x] = 0;
		}
	}
	
	
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
		int mines = cMain::defaultMines;
		cMain::minecount=mines;
		
		while (mines)
		{
			int rx = rand() % cMain::nFieldWidth;
			int ry = rand() % cMain::nFieldHeight;
			
			if(cMain::nField[ry * cMain::nFieldWidth + rx] == 0 && rx != x && ry != y) //generates random mines
			{
				cMain::nField[ry * cMain::nFieldWidth + rx] = -1;
				mines--;
			}
		}
		
		
		
		m_timer.Start(1000);
		
		cMain::bFirstClick = false;
	}
	
	
	
	if(cMain::nField[y * cMain::nFieldWidth + x] == -1 && cMain::mineloc[y * cMain::nFieldWidth + x] != 1)	//if mine is hit
	{
		wxMessageBox("There was a mine");
		cMain::bFirstClick = true;  //reset click (for gen)
		
		for(int x=0;x<cMain::nFieldWidth;x++) //reset board
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
	else
	{
		//counts neighboring mines
		int mine_count = 0;
		
		for(int i=-1; i<2; i++)
		{
			for(int j = -1; j<2; j++)
			{
				if(x + i >= 0 && x + i < cMain::nFieldWidth && y + j >=0 && y + j < cMain::nFieldHeight) //not to search out of board
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
			btn[y * cMain::nFieldWidth + x] -> SetLabel(std::to_string(mine_count)); //how many mines
			
			btn[y*cMain::nFieldWidth + x] -> Enable(false);
		}

	}
	
	if(minecount==0)
	{
		wxMessageBox(wxString::Format("You Won!\nYour time was: %d s.", time));
		
		std::ofstream highscores;
		highscores.open("highscores.dat", std::ios_base::app);
		//highscores<<"Player: "<<cMain::name<<", Dimensions:"<<nFieldWidth<<" x "<<nFieldHeight<<", Time: "<<time<<" s.\n";
		highscores<<cMain::name<<' '<<nFieldWidth<<' '<<nFieldHeight<<' '<<time<<'\n';
		highscores.close();
		
		//reset //unneeded?
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
	
	//update minecount
	SetStatusText(_("Mines left: "+std::to_string(minecount)), 1);
	
	evt.Skip();

}

void cMain::rclick(wxMouseEvent &evt)	//flag a mine
{	
	int x = (evt.GetId() - 10000) % cMain::nFieldWidth;
	int y = (evt.GetId() - 10000) / cMain::nFieldWidth;
	
	
	
	if(cMain::mineloc[y * cMain::nFieldWidth + x] == 0) //flag //NEEDS MORE IFS FOR BUGFIXING
	{
		cMain::mineloc[y * cMain::nFieldWidth + x] = 1;
		btn[y * cMain::nFieldWidth + x] -> SetLabel("M");
	}
	else //unflag
	{
		cMain::mineloc[y * cMain::nFieldWidth + x] = 0;
		btn[y * cMain::nFieldWidth + x] -> SetLabel("");
	}
	if(cMain::mineloc[y * cMain::nFieldWidth + x] == 1)  // && cMain::nField[y * cMain::nFieldWidth + x] == -1
	{
		cMain::minecount--;
	}else{
		cMain::minecount++;
	}
	
	
	//update minecount
	SetStatusText(_("Mines left: "+std::to_string(minecount)), 1);
	
	evt.Skip();
}

void cMain::OnTimer(wxTimerEvent &evt)
{
	time++;
	
	SetStatusText(_("Time: "+std::to_string(time)+" s"));
	
	evt.Skip();
}

#include <string>
#include <fstream>
#include <ctime> //unneeded?
#include "wx/wx.h"
#include "cMain.h"

#define inputOn 1
#define speclow 3
#define specupp 20

//dont confuse minecount with mine_count

/* gameplay bugs(features):
 set minecount to 0 and press any (even flagged) tile to win
 after losing flag all mines and click on one to win
 
*/

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_MENU(9001, cMain::OnMenuNewGame)
EVT_MENU(9002, cMain::OnMenuHighscores)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "MS1.2", wxPoint(0,0), wxSize(800,600))
{	
	if(inputOn){
	bool invalid=1;
	
	std::cout<<"Please provide your name:\n";
	std::cin>>cMain::name;
	while (invalid){
		std::cout<<"Please provide the dimensions you want to play with (From 3-20):\n";
		std::cin>>cMain::nFieldWidth>>cMain::nFieldHeight;
		if(cMain::nFieldWidth>specupp || cMain::nFieldWidth<speclow || cMain::nFieldHeight>specupp || cMain::nFieldHeight<speclow){
		std::cout<<"Unable to construct the board with your specifications\n";
		}else{invalid=0;}
	}
	invalid=1;
	int bombupp=cMain::nFieldWidth*cMain::nFieldHeight/2;
	while (invalid){
		std::cout<<"Please provide the amount of mines you want to play with (From 1-"<<bombupp<<"):\n";
		std::cin>>cMain::defaultMines;
		if(cMain::defaultMines<1 || cMain::defaultMines>bombupp){
			std::cout<<"Unable to construct the board with your specifications\n";
		}else{invalid=0;}
	}
	}
	
	//Options
	wxMenuBar *menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu();
	menuBar->Append(fileMenu, _("Menu"));
	SetMenuBar(menuBar);
	fileMenu->Append(9001, _("New game"));  //magic number
	fileMenu->Append(9002, _("Highscores"));

	//timer
	m_timer.SetOwner(this);
	Bind(wxEVT_TIMER, &cMain::OnTimer, this, m_timer.GetId());
	wxStatusBar *statusBar = CreateStatusBar(2); //2 = size
	
	//status init
	SetStatusText(_("Mines left: "+std::to_string(cMain::defaultMines)), 1);
	SetStatusText(_("Time: "+std::to_string(0)+" s"));
	
	//creates button grid
	btn = new wxButton*[cMain::nFieldWidth * cMain::nFieldHeight]; 
	
	wxGridSizer *grid = new wxGridSizer(cMain::nFieldWidth, cMain::nFieldHeight, 0, 0);  //generates a sizer which corrects positions of buttons
	
	
	cMain::nField = new int[cMain::nFieldWidth * cMain:: nFieldHeight];  //board; -1 is bomb, 0 is not
	cMain::mineloc = new int[cMain::nFieldWidth * cMain:: nFieldHeight];  //flag positions
	
	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
	
	for(int x=0;x<cMain::nFieldWidth;x++)
	{
		for(int y=0;y<cMain::nFieldHeight;y++)
		{
			cMain::btn[y * (cMain::nFieldWidth) + x] = new wxButton(this, 10000 + (y * (cMain::nFieldWidth) + x));  //10000 is magic id
			
			cMain::btn[y * (cMain::nFieldWidth) + x]->SetFont(font);
			
			grid -> Add(btn[y * (cMain::nFieldWidth) + x], 1, wxEXPAND | wxALL);  //adjusts button size
			cMain::btn[y * cMain::nFieldWidth + x] -> Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			cMain::btn[y * cMain::nFieldWidth + x] -> Bind(wxEVT_RIGHT_DOWN, &cMain::rclick, this);
			cMain::nField[y * cMain::nFieldWidth + x] = 0;
			cMain::mineloc[y * cMain::nFieldWidth + x] = 0;
		}
	}
	
	
	this -> SetSizer(grid);
	grid -> Layout();
	
}

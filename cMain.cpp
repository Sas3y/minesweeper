#include "cMain.h"
#include <string>
#include <fstream>

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
		cMain::showEnd();
		
	}
	else
	{
		//reveals 0's around
		cMain::reveal0(x,y);
		
		revealTile(x,y);
		//counts neighboring mines
		//moved code to revealTile
		
		
	}
	
	if(minecount==0)
	{
		m_timer.Stop();
		wxMessageBox(wxString::Format("You Won!\nYour time was: %d s.", time));
		
		std::ofstream highscores;
		highscores.open("highscores.dat", std::ios_base::app);
		//highscores<<"Player: "<<cMain::name<<", Dimensions:"<<nFieldWidth<<" x "<<nFieldHeight<<", Time: "<<time<<" s.\n";
		highscores<<cMain::name<<' '<<nFieldWidth<<' '<<nFieldHeight<<' '<<defaultMines<<' '<<time<<'\n';
		highscores.close();
		
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
		btn[y * cMain::nFieldWidth + x] -> SetLabel("F");
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
	
	if(time%cMain::difficulty==0){
	cMain::botPlace();
	}
	
	SetStatusText(_("Time: "+std::to_string(time)+" s"));
	
	evt.Skip();
}

void cMain::reset(){
	cMain::bFirstClick = true;
	time=0;
	SetStatusText(_("Time: "+std::to_string(time)+" s"));
}

void cMain::OnMenuNewGame(wxCommandEvent &evt){

	m_timer.Stop();
	cMain::reset();
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
	
	//unneeded?
	cMain::bFirstClick=1;
	
	wxMessageBox(wxString::Format("Board reset"));
}


void cMain::OnMenuHighscores(wxCommandEvent &evt){
	
	std::ifstream hsR; //hs for highscores, R for readable
	hsR.open("highscores.dat");
	//NAME DIM1 DIM2 BOMBS TIME
	
	//init best 3
	int HSdim1, HSdim2, HSbombs;
	std::string HSname1="-\n",HSname2="-\n",HSname3="-\n",HSname;
	int HStime1=999, HStime2=999, HStime3=999, HStime;
	
	while(hsR >> HSname >> HSdim1 >> HSdim2 >> HSbombs >> HStime){ //checks if not end of file
		//hsR>>HSname >> HSdim1 >> HSdim2 >> HSbombs >> HStime;
		//TEST
		//std::cout<<HSname<<HSdim1<<HSdim2<<HSbombs<<HStime<<std::endl;
		//if checks if line matches with currect settings
		if( ((HSdim1==cMain::nFieldWidth && HSdim2==cMain::nFieldHeight)||(HSdim2==cMain::nFieldWidth && HSdim1==cMain::nFieldHeight))&&(HSbombs==cMain::defaultMines) ){
			if(HStime<HStime1){//if HStime (file) is better then HStime1, moves HStime1 down one (same with HStime2) and HStime1 is replaced with HStime
			HStime3=HStime2;
			HStime2=HStime1;
			HStime1=HStime;
			HSname3=HSname2;
			HSname2=HSname1;
			HSname1=HSname;
			}else if(HStime<HStime2){
			HStime3=HStime2;
			HStime2=HStime;
			HSname3=HSname2;
			HSname2=HSname;
			}else if(HStime<HStime3){
			HStime3=HStime;
			HSname3=HSname;
			}
		}
	}
	
	//formatting strings before box for "simplicity"
	if(HSname1!="-\n"){
		HSname1="1) "+HSname1+": "+std::to_string(cMain::nFieldWidth)+"x"+std::to_string(cMain::nFieldHeight)+" with "+std::to_string(cMain::defaultMines)+" bombs time: "+std::to_string(HStime1)+"\n";
	}
	if(HSname2!="-\n"){
		HSname2="2) "+HSname2+": "+std::to_string(cMain::nFieldWidth)+"x"+std::to_string(cMain::nFieldHeight)+" with "+std::to_string(cMain::defaultMines)+" bombs time: "+std::to_string(HStime2)+"\n";
	}
	if(HSname3!="-\n"){
		HSname3="3) "+HSname3+": "+std::to_string(cMain::nFieldWidth)+"x"+std::to_string(cMain::nFieldHeight)+" with "+std::to_string(cMain::defaultMines)+" bombs time: "+std::to_string(HStime3);
	}else{HSname3="-";}//to remove \n
	
	wxMessageBox("Highscores for your dimensions and bombs:\n"+HSname1+HSname2+HSname3);
}

void cMain::botPlace(){
	
	int tries=100; //magic number
	bool isClosed=1;
	while (tries!=0){
	
		//generates random coords
		int rx = rand() % cMain::nFieldWidth;
		int ry = rand() % cMain::nFieldHeight;
		
		//checks if surrounding is all enabled buttons
		for(int i=-1; i<2; i++){
		for(int j=-1; j<2; j++){
			if((rx+i) >= 0 && (rx+i) < cMain::nFieldWidth && (ry+j) >= 0 && (ry+j) < cMain::nFieldHeight){
				if(!(btn[(ry+j)*cMain::nFieldWidth+(rx+i)]->IsEnabled())){isClosed=0;} //BUGGED (5x8)
				//std::cout<<"COORDS: "<<rx+i<<" x "<<ry+j<<std::endl;
			}
		}
		}
		
			
		//places a mine in an undiscovered (by sides) tile
		if(isClosed){
			if(cMain::nField[ry * cMain::nFieldWidth + rx] == 0)//if mine isnt there
			{
				
				cMain::nField[ry * cMain::nFieldWidth + rx] = -1;
				cMain::minecount++;
				SetStatusText(_("Mines left: "+std::to_string(minecount)), 1); //update minecount statusbar
				
				//FOR TESTS TESTING
				std::cout<<"Bot placed on "<<ry * cMain::nFieldWidth+rx<<std::endl;
				
				break;
			}
		}else{tries--;isClosed=1;}
	}
	
}

void cMain::reveal0(int x, int y){

	bool is0=1; //bool if the tile is 0
	int tempx, tempy; //for easier reading
	for(int a=-1; a<2; a++){ //2fors search surrounding
	for(int b=-1; b<2; b++){
		tempx = x + a;
		tempy = y + b;
		if(tempx >= 0 && tempx < cMain::nFieldWidth && tempy >= 0 && tempy < cMain::nFieldHeight){
			if(btn[tempy*cMain::nFieldWidth+tempx]->IsEnabled()){
				for(int i=-1; i<2; i++){
				for(int j=-1; j<2; j++){
					tempx = x + i + a;
					tempy = y + j + b;
					if(tempx >= 0 && tempx < cMain::nFieldWidth && tempy >= 0 && tempy < cMain::nFieldHeight){
						if(cMain::nField[(tempy) * cMain::nFieldWidth + (tempx)] == -1){is0=0;}
					}
				}}
			
				
				if(is0){
					
					tempx = x + a;
					tempy = y + b;
					btn[tempy * cMain::nFieldWidth + tempx] -> SetLabel(std::to_string(0)); //how many mines
					btn[tempy * cMain::nFieldWidth + tempx] -> Enable(false);//disables clicking the button
					cMain::reveal0(tempx,tempy); //RECURSIVE!
					
					//I think this needs to be put after reveal0, because it wouldnt check false 0's otherwise
					for(int i=-1; i<2; i++){//reveals every tile around is0
					for(int j=-1; j<2; j++){
						tempx = x + i + a;
						tempy = y + j + b;
						if(tempx >= 0 && tempx < cMain::nFieldWidth && tempy >= 0 && tempy < cMain::nFieldHeight){
							cMain::revealTile(tempx,tempy);
						}
					}}

				}else{is0=1;}
			}
		}
	}}
	
}

void cMain::revealTile(int x, int y){
	
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
		
		btn[y*cMain::nFieldWidth + x] -> Enable(false);//disables clicking the button
	}
	
}

void cMain::showEnd(){


	//cMain::nField[y * cMain::nFieldWidth + x] == -1 && cMain::mineloc[y * cMain::nFieldWidth + x] != 1
	
	//-1 for bomb
	for(int x=0;x<cMain::nFieldWidth;x++){
	for(int y=0;y<cMain::nFieldHeight;y++){
		
		if(cMain::nField[y * cMain::nFieldWidth + x]==-1){
			//bomb
			if(cMain::mineloc[y * cMain::nFieldWidth + x]==0){
			btn[y * cMain::nFieldWidth + x] -> SetLabel("x"); //missed bomb
			}else{btn[y * cMain::nFieldWidth + x] -> SetLabel("F");}
		}else{
			if(cMain::mineloc[y * cMain::nFieldWidth + x]==0){
				//no bomb
				cMain::revealTile(x,y);
			}else{
			btn[y * cMain::nFieldWidth + x] -> SetLabel("f");
			btn[y * cMain::nFieldWidth + x] -> Enable(false);
			}
			
		}
		/*Legend:
		number - how many bombs around
		f - wrongly marked tile
		x - unmarked bomb
		F - marked bomb
		*/
		
		
	}}
	m_timer.Stop();
	wxMessageBox("You lost -\nthere was a mine there");
	
	//moved resetting to New Game button
}











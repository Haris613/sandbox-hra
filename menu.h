//
// Created by haris on 21.5.17.
//
#ifndef SEMESTRALKA_MENU_H
#define SEMESTRALKA_MENU_H


#include <string>
#include <ncurses.h>

using namespace std;


enum menuPicksConsts {NEW_GAME, LOAD_GAME, CONTROLS, EXIT};

#define MOVE_UP -1
#define MOVE_DOWN 1

#define ENTER_KEY 10
#define BACKSPACE_KEY 7


void initNcurses();
WINDOW * initMenu();
void drawMenu(WINDOW * menu,const string (&menuPicks)[4]);
void moveCursor(WINDOW * menu, int & currPick, const int moveDir, const string (&menuPicks)[4]);

void generateMap(const char * fileName);
void startGame(WINDOW * menu, char * fileName);

void newGame(WINDOW * menu);// todo use loadGame in this probably - after the new game is created
void loadGame(WINDOW * menu);//todo maybe change to game ID, depends how ill solve loading games
void controls(WINDOW * menu);
void closeMenu(WINDOW * menu);

void menu(){
	initNcurses();

	const string menuPicks[4] = {"New game", "Load game", "Controls", "Exit"};
	WINDOW * menu = initMenu();
	int currPick=NEW_GAME;
	drawMenu(menu,menuPicks);

	while(true) {
		int input = wgetch(menu);
		if( input == KEY_UP && currPick>NEW_GAME )
			moveCursor(menu,currPick,MOVE_UP,menuPicks);
		if( input == KEY_DOWN && currPick<EXIT )
			moveCursor(menu,currPick,MOVE_DOWN,menuPicks);


		if( input == ENTER_KEY ){
			werase(menu);
			wrefresh(menu);
			switch (currPick) {
				case NEW_GAME:
					newGame(menu);
					break;
				case LOAD_GAME:
					loadGame(menu);
					break;
				case CONTROLS:
					controls(menu);
					break;
				case EXIT:
					closeMenu(menu);
					endwin();
					return;
				default:
					throw "Menu wrong currPick";
			}
			currPick = NEW_GAME;
			drawMenu(menu,menuPicks);
		}
	}
}

void initNcurses(){
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	if( has_colors() )
		start_color();
	refresh();
}

WINDOW * initMenu(){
	WINDOW * menu = newwin(20, 40, 1, COLS/2-20);

	keypad( menu, TRUE );
	return menu;
}

void drawMenu(WINDOW * menu, const string (&menuPicks)[4]){
	box(menu,0,0);

	wattron(menu, A_REVERSE);
	mvwprintw(menu,1,3,menuPicks[NEW_GAME].c_str());
	wattroff(menu, A_REVERSE);

	for (int i = LOAD_GAME; i <= EXIT; ++i)
		mvwprintw(menu,i*2+1,3,menuPicks[i].c_str());
}

void moveCursor(WINDOW * menu, int & currPick, const int moveDir, const string (&menuPicks)[4]){

	mvwprintw(menu,currPick*2+1,3,menuPicks[currPick].c_str());

	currPick+=moveDir;

	wattron(menu, A_REVERSE);
	mvwprintw(menu,currPick*2+1,3,menuPicks[currPick].c_str());
	wattroff(menu, A_REVERSE);
}

void generateMap(const char * fileName){

}

void startGame(WINDOW * menu, char * fileName){

}

void newGame(WINDOW * menu){
	char fileName[26];
	bool warning=false;
	halfdelay(7);
	mvwprintw(menu,1,1,"Input name of map:");
	mvwprintw(menu,4,1,"Usable characters are: \n letters, numbers, spaces,\n dashes and underscores");
	box(menu,0,0);
	int pos=0;
	for( int i=0 ;true; ++i ){
		char input = (char) wgetch(menu);
		if(warning) {
			for (int j = 1; j < 39; ++j)
				mvwprintw(menu, 8, j , " ");
			warning=false;
		}

		if( isalpha(input) || isdigit(input) || input==' ' || input=='_' || input=='-' ) {
			if(pos!=24) {
				mvwprintw(menu, 2, (pos++) + 1, "%c", input);
				fileName[pos] = input;
			}
			else{
				mvwprintw(menu, 8, 1, "Name cant be longer than 25 characters");
				warning=true;
			}
		}
		else if(input == BACKSPACE_KEY){
			mvwprintw(menu,2,pos+1," ");
			if(pos!=0)
				--pos;
		}
		else if(input == ENTER_KEY){
			if(pos==0){
				mvwprintw(menu, 8,1, "Name cant be empty");
				warning=true;
				continue;
			}
			fileName[pos+1]='\0';
			break;
		}
		else if(input!=ERR){
			mvwprintw(menu, 8,1, "You cant use that character");
			warning=true;
		}

		if(i%2)
			mvwprintw(menu,2,pos+1," ");
		else
			mvwprintw(menu,2,pos+1,"_");
		wrefresh(menu);
	}
	cbreak();//turning the halfdelay function off
	werase(menu);
	box(menu,0,0);
	mvwprintw(menu,2,2,"Generating map...");
	wrefresh(menu);
	generateMap(fileName);
	wgetch(menu);

	mvwprintw(menu,2,2,"Starting the game...");//todo move to startGame()
	wrefresh(menu);
	wgetch(menu);
	startGame(menu, fileName);
	werase(menu);
	wrefresh(menu);
}

void loadGame(WINDOW * menu){
	char fileName[] = "newMap";
	startGame(menu, fileName);
}

void controls(WINDOW * menu){

	werase(menu);
	wrefresh(menu);
}

void closeMenu(WINDOW * menu){
	werase(menu);
	wrefresh(menu);
	delwin(menu);
}

#endif //SEMESTRALKA_MENU_H

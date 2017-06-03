//
// Created by haris on 21.5.17.
//
#ifndef SEMESTRALKA_MENU_H
#define SEMESTRALKA_MENU_H


#include <string>
#include <ncurses.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "air.h"
#include "stone.h"
#include "dirt.h"
#include "water.h"
#include "lava.h"

using namespace std;




void initNcurses();
WINDOW * initMenu();
void drawMenu(WINDOW * menu,const string (&menuPicks)[4]);
void moveCursor(WINDOW * menu, int & currPick, const int moveDir, const string (&menuPicks)[4]);

tiles * surfaceTile();
tiles * undergroundTile();
tiles *** loadMap(const char * fileName);
tiles *** generateMap(const char * fileName);
void startGame(WINDOW * menu, tiles *** map);

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
	werase(menu);
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

/**
 * @brief generate tile for the surface part of map
 * @details this function generates tile randomly with adjusted probability, that is set to match the surface environment
 * the probabilities are approximately as follows: dirt - 43.8%, water - 25.3%, stone - 16.9%, air - 10%, lava - 4%
 * @return pointer to tile that has been generated
 */
tiles * surfaceTile() {
	srand((unsigned int) time(0));//making the number more "random"
	int randomTile = rand()%TILE_TYPE_END;//generate random number from tileType range
	switch (randomTile){//according to the generated number decide what tile is to be generated
		case AIR: //half times it will be air, the other half will be for third of cases stone and for the rest of cases dirt
			return ((rand() % 2) ? ((rand() % 3) ? (tiles *)(new dirt) : (tiles *)(new stone)) : (tiles *)(new air));
		case DIRT://always dirt
			return (tiles *)(new dirt);
		case STONE://half times it will be truly stone, the other half dirt
			return ((rand() % 2) ? (tiles *)(new stone) : (tiles *)(new dirt));
		case WATER://always water
			return (tiles *)(new water);
		case LAVA: //one fifth of cases it will be lava, otherwise for third of these cases lava, otherwise for third of these cases stone and otherwise dirt
			return ((rand() % 5) ? ((rand() % 3) ? ((rand() % 3) ? (tiles *)(new dirt) : (tiles *)(new stone) ) : (tiles *)(new water)) : (tiles *)(new lava));
		default:
			return (tiles *)(new dirt);//just in case, but this should never be used
	}
}

/**
 * @brief generate tile for the underground part of map
 * @details this function generates tile randomly with adjusted probability, that is set to match the underground environment
 * the probabilities are approximately as follows: stone - 48.9%, lava - 24.4%, dirt - 13.3%, water - 6.7%, air - 6.7%
 * @return pointer to tile that has been generated
 */
tiles * undergroundTile() {
	srand((unsigned int) time(0));//making the number more "random"
	int randomTile = rand()%TILE_TYPE_END;//generate random number from tileType range
	switch (randomTile){//according to the generated number decide what tile is to be generated
		case AIR://third of cases it will be air, otherwise for quarter of cases dirt, otherwise dirt
			return ((rand() % 3) ? ((rand() % 4) ? (tiles *)(new stone) : (tiles *)(new dirt)) : (tiles *)(new air));
		case DIRT://half times it will be truly dirt, the other half stone
			return ((rand() % 2) ? (tiles *)(new dirt) : (tiles *)(new stone));
		case STONE://always stone
			return (tiles *) (new stone);
		case WATER://third of cases it will truly be water, otherwise for third of cases lava, otherwise stone
			return ((rand() % 3) ? ((rand() % 3) ? (tiles *)(new stone) : (tiles *)(new lava) ) : (tiles *)(new water));
		case LAVA://always lava
			return (tiles *)(new lava);
		default:
			return (tiles *)(new stone);//just in case, but this should never be used
	}
}


tiles *** generateMap(const char * fileName){
	ofstream mapFile;
	mapFile.open(fileName);
	if(! mapFile.is_open() )
		return NULL;
	tiles ***map = new tiles**[20];
	for (int i = 0; i < 20; ++i) {
		map[i]=new tiles*[100];
	}
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 100; ++j) {
			map[i][j] = (tiles *) ((i < 8) ? (new air) : ((i < 14) ? surfaceTile() : undergroundTile()));
			map[i][j]->saveState(mapFile);
		}

	mapFile.close();
	return map;/*
	//todo
	for (int i = 0; i < 20; ++i) //todo make this function return map pointer and then pass it to the new startGame function(or with other name) where it works with this array and then deletes it
		for (int j = 0; j < 100; ++j)// todo possibly make a new class called game and put it to its constructor..
			delete map[i][j];
	for (int i = 0; i < 20; ++i) {
		delete[] map[i];
	}
	delete[] map;
	return NULL;*/
}


void startGame(WINDOW * menu, tiles *** map){

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
				mvwprintw(menu, 2, (pos) + 1, "%c", input);
				fileName[pos] = input;
				++pos;
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
			fileName[pos]='\0';
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

	tiles *** map = generateMap(fileName);
	if(!map){
		mvwprintw(menu,2,2,"Error! File could not be generated.");
		wattron(menu, A_REVERSE);
		mvwprintw(menu,4,2,"OK");
		wattroff(menu, A_REVERSE);
		wrefresh(menu);

		for(char test = '\0'; test!=ENTER_KEY ; )
			test = (char) wgetch(menu);

		return;
	}

	mvwprintw(menu,2,2,"Starting the game...");//todo move to startGame()
	wrefresh(menu);
	wgetch(menu);//todo delete
	startGame(menu, map);
}


tiles *** loadMap(const char * fileName) {
	return null;
}

void loadGame(WINDOW * menu){//todo make some kind of interface for the player to pick saved game from save folder
	char fileName[] = "newMap";
	startGame(menu, loadMap(fileName) );
}

void controls(WINDOW * menu){

}

void closeMenu(WINDOW * menu){
	werase(menu);
	wrefresh(menu);
	delwin(menu);
}


#endif //SEMESTRALKA_MENU_H

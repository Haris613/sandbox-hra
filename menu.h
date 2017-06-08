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
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <vector>

#include "air.h"
#include "stone.h"
#include "dirt.h"
#include "water.h"
#include "lava.h"

using namespace std;




void initNcurses();
WINDOW * initMenu();
void drawMenu(WINDOW * menu,const vector<string> & menuPicks, const int displayMode);
void moveCursor(WINDOW * menu, unsigned int & currPick, const int moveDir, const vector<string> & menuPicks, const int displayMode);

tiles * surfaceTile();
tiles * undergroundTile();
tiles *** generateMap(const char * path);

tiles * resolveTile(const uint8_t tileType);
tiles *** loadMap(const char * fileName);

void startGame(WINDOW * menu, tiles *** map);

void newGame(WINDOW * menu);// todo use loadGame in this probably - after the new game is created
void loadGame(WINDOW * menu);//todo maybe change to game ID, depends how ill solve loading games
void controls(WINDOW * menu);
void closeMenu(WINDOW * menu);

void menu(){
	initNcurses();

	const vector<string> menuPicks = {"New game", "Load game", "Controls", "Exit"};
	WINDOW * menu = initMenu();
	unsigned int currPick=NEW_GAME;
	drawMenu(menu,menuPicks,EVEN_LINES);

	while(true) {
		int input = wgetch(menu);
		if( input == KEY_UP && currPick>NEW_GAME )
			moveCursor(menu,currPick,MOVE_UP,menuPicks,EVEN_LINES);
		if( input == KEY_DOWN && currPick<EXIT )
			moveCursor(menu,currPick,MOVE_DOWN,menuPicks,EVEN_LINES);


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
			drawMenu(menu,menuPicks,EVEN_LINES);
		}
	}
}

/**
 * @brief initializes ncurses standard screen and sets attributes to desired values
 * @details First initializes the screen, then noecho is set so user input isnt printed on the screen right away and
 * probably at bad coordinates. Cbreak makes the program wait for user input every time getch or wgetch is called.
 * curs_set(0) hides the cursor. If the terminal can draw colors, initialize them. Refresh draws empty screen.
 */
void initNcurses(){
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	if( has_colors() )
		start_color();
	refresh();
}

/**
 * @brief initializes menu
 * @details Initializes menu window with sizes given in WINDOW_HEIGHT and WINDOW_WIDTH constants,
 * the menu is in the middle of the screen. The keypad option in this window is set on to register
 * function keys such as arrow keys.
 * @return pointer to the menu window
 */
WINDOW * initMenu(){
	WINDOW * menu = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 1, COLS/2-WINDOW_WIDTH/2);

	keypad( menu, TRUE );
	return menu;
}

/**
 * @brief draws a menu given by parameter
 * @details Clears the screen and prints all choices in the menuPick vector. The first one has reverted colors,
 * because its the one currently being chosen.
 * @param menu pointer to the menu window
 * @param menuPicks "list" of the menu choices
 * @param displayMode decides how far away from each other are lines supposed to be
 */
void drawMenu(WINDOW * menu, const vector<string> & menuPicks, const int displayMode){
	werase(menu);
	box(menu,0,0);

	if(menuPicks.empty())
		return;

	wattron(menu, A_REVERSE);
	mvwprintw(menu,1,3,menuPicks[0].c_str());
	wattroff(menu, A_REVERSE);

	for (size_t i = 1, size = menuPicks.size() ; i < size && i*displayMode < WINDOW_HEIGHT ; ++i)
		mvwprintw(menu,i*displayMode+1,3,menuPicks[i].c_str());

	wrefresh(menu);
}

/**
 * @brief moves the "cursor" in the menu
 * @details
 * @param menu pointer to the menu window
 * @param currPick position of the currently chosen item from the "list", its a reference because its being changed in this function
 * @param moveDir direction the cursor is moving in (up -1, down 1)
 * @param menuPicks "list" of the menu choices
 * @param displayMode decides how far away from each other are lines supposed to be
 */
void moveCursor(WINDOW * menu,unsigned  int & currPick, const int moveDir, const vector<string> & menuPicks, const int displayMode){

	mvwprintw(menu,currPick*displayMode+1,3,menuPicks[currPick].c_str());

	currPick+=moveDir;

	wattron(menu, A_REVERSE);
	mvwprintw(menu,currPick*displayMode+1,3,menuPicks[currPick].c_str());
	wattroff(menu, A_REVERSE);
	//todo currPick can go out of bounds(graphical bounds) with more than 18 files in map folder
}

/**
 * @brief generate tile for the surface part of map
 * @details this function generates tile randomly with adjusted probability, that is set to match the surface environment
 * the probabilities are approximately as follows: dirt - 43.8%, water - 25.3%, stone - 16.9%, air - 10%, lava - 4%
 * @return pointer to tile that has been generated
 */
tiles * surfaceTile() {
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

/**
 * @brief generates new map randomly
 * @details Creates a new directory with maps if there already isnt one, then opens a new file in this directory.
 * Creates a new matrix of "random" tiles with desired chances and writes them to this file.
 * @param path name of the newly generated file with a path to it (its in the map directory)
 * @return pointer to the map matrix that has been generated, if its null, the generation has failed
 */
tiles *** generateMap(const char * path){
	srand((unsigned int) time(0));//makes the tiles more "random"
	mkdir("map", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	ofstream mapFile;
	mapFile.open(path, ios::binary);
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
	return map;
}


void startGame(WINDOW * menu, tiles *** map){

}

/**
 * @brief lets player input name for his game, generates it and starts it
 * @details The game is generated to the map directory (its created in generateMap if it doesnt exist).
 * Player can input letters, numbers, spaces, dashes and underscores. Anything other than that
 * is an illegal character and the player is warned with a warning message. Name also cant be empty
 * and cant be longer than 25 characters, the player is warned in this case as well.
 * Both of these warning messages are cleared on next passage of the loop where the input happens.
 * The loop doesnt wait for the players input to continue, every 0.7 seconds it does that itself.
 * This occurs for the cursor to blink so the player knows where he ended his input.
 * If the input is a legal input character its written to the path array, printed to the screen
 * and the cursor moves as well. This doesnt happen if the name is 25 characters long.
 * If the player pressed the backspace key, the last character is erased from the screen
 * and the cursor is moved. This doesnt happen when the name is 0 characters long.
 * Its not deleted from the array because it will either be overwritten or it wont be in the final "string"
 * because of the terminating character(\0) that is added after input is finished.
 * The input ends when the player presses enter. Then the file is generated and that is reported to the player.
 * If the generation didnt go as planned the player is warned and after hitting the enter key it goes back to the menu.
 * Otherwise the game is started.
 * @param menu pointer to the menu window
 */
void newGame(WINDOW * menu){
	char path[30]="map/";
	bool warning=false;
	halfdelay(7);
	mvwprintw(menu,1,1,"Input name of map:");
	mvwprintw(menu,4,1,"Usable characters are: \n letters, numbers, spaces,\n dashes and underscores");
	box(menu,0,0);
	int pos=4;
	for( int i=0 ;true; ++i ){
		char input = (char) wgetch(menu);
		if(warning) {
			for (int j = 1; j < 39; ++j)
				mvwprintw(menu, 8, j , " ");//clears whole line 8 except menus border.
			warning=false;
		}

		if( isalpha(input) || isdigit(input) || input==' ' || input=='_' || input=='-' ) {
			if(pos!=29) {
				mvwprintw(menu, 2, (pos-4) + 1 , "%c", input);
				path[pos] = input;
				++pos;
			}
			else{
				mvwprintw(menu, 8, 1, "Name cant be longer than 25 characters");
				warning=true;
			}
		}
		else if(input == BACKSPACE_KEY){
			mvwprintw(menu,2,pos-4+1," ");
			if(pos!=4)
				--pos;
		}
		else if(input == ENTER_KEY){
			if(pos==4){
				mvwprintw(menu, 8,1, "Name cant be empty");
				warning=true;
				continue;
			}
			path[pos]='\0';
			break;
		}
		else if(input!=ERR){ //ERR is generated when the player doesnt input anything within the halfdelay time period
			mvwprintw(menu, 8,1, "You cant use that character");
			warning=true;
		}

		if(i%2)		//produces the blink effect when the player doesnt input anything
			mvwprintw(menu,2,pos-4+1," ");
		else
			mvwprintw(menu,2,pos-4+1,"_");
		wrefresh(menu);
	}
	cbreak();//turning the halfdelay function off
	werase(menu);
	box(menu,0,0);
	mvwprintw(menu,2,2,"Generating map...");
	wrefresh(menu);

	tiles *** map = generateMap(path);
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

/**
 * @brief makes a tile from given tileType_t
 * @param tileType type of tile with designated value
 * @return pointer to newly created tileType, NULL if wrong number has been given
 */
tiles * resolveTile(const uint8_t tileType){
	switch(tileType){
		case AIR:
			return new air;
		case DIRT:
			return new dirt;
		case STONE:
			return new stone;
		case WATER:
			return new water;
		case LAVA:
			return new lava;
		default:
			return NULL;
	}
}

/**
 * @brief loads map matrix from given file
 * @details Opens file in map directory for read, creates a new matrix of tiles pointers and fills them
 * with pointers to types read from the file.
 * @param fileName name of the file the game is loaded from
 * @return pointer to newly created tile matrix, NULL if something went wrong (file couldnt be open, there was illegal value inside)
 */
tiles *** loadMap(const char * fileName) {
	string path = "map/";
	path+=fileName;
	ifstream mapFile;
	mapFile.open(path.c_str(),ios::binary);
	if(! mapFile.is_open() )
		return NULL;

	tiles ***map = new tiles**[20];
	for (int i = 0; i < 20; ++i) {
		map[i]=new tiles*[100];
	}
	for (int i = 0; i < 20; ++i)
		for (int j = 0; j < 100; ++j) {
			uint8_t tileType;
			mapFile.read((char*)&tileType,sizeof(tileType));
			map[i][j]=resolveTile(tileType);

			if(!map[i][j]) {
				for (int k = 0; k < i; ++k)
					for (int l = 0; l < j; ++l)
						delete map[k][l];
				for (int k = 0; k < 20; ++k)
					delete[] map[k];
				delete[] map;
				return NULL;
			}

		}
	return map;
}

/**
 * @brief lets player pick a file with saved game from map folder
 * @details First the function searches through the map directory and adds all files to the files vector.
 * If the directory doesnt exist the vector will be empty as expected. In this list are exluded all files
 * starting with '.' which includes mainly systems '.' and '..' but also hidden files. The player cant input
 * file starting with '.' because its an illegal character. Menu with all these files is drawn upon the screen,
 * one item per line. The player then chooses from these items with the use of arrow keys up and down and confirms
 * his choice with enter key, after which the file is attempted to load. If it fails the the player is warned
 * and after hitting the enter key it goes back to the menu. Otherwise the game is started.
 * @param menu pointer to the menu window
 */
void loadGame(WINDOW * menu){
	box(menu,0,0);
	vector<string> files;
	DIR * mapDir;
	struct dirent * mapFile;
	if((mapDir = opendir ("map")) != NULL) {
		while((mapFile = readdir (mapDir)) != NULL) {//todo all files including those that contain illegal characters are shown here
			if(mapFile->d_name[0]!='.')
				files.push_back(mapFile->d_name);
		}
		closedir (mapDir);
	}
	drawMenu(menu,files,ALL_LINES);

	tiles *** map;
	unsigned int currPick=0;
	while(true) { //todo this could be merged to one unified function with the menu (it would return currPick)
		int input = wgetch(menu);
		if (input == KEY_UP && currPick > 0)
			moveCursor(menu, currPick, MOVE_UP, files, ALL_LINES);
		if (input == KEY_DOWN && currPick < files.size()-1)
			moveCursor(menu, currPick, MOVE_DOWN, files, ALL_LINES);


		if (input == ENTER_KEY) {
			map = loadMap(files[currPick].c_str());
			break;
		}
	}

	if(!map){
		werase(menu);
		box(menu,0,0);
		mvwprintw(menu,2,2,"Error! File could not be read.");
		wattron(menu, A_REVERSE);
		mvwprintw(menu,4,2,"OK");
		wattroff(menu, A_REVERSE);
		wrefresh(menu);

		for(char test = '\0'; test!=ENTER_KEY ; )
			test = (char) wgetch(menu);
		return;
	}
	mvwprintw(menu,4,2,"OK");
	wattroff(menu, A_REVERSE);
	wrefresh(menu);

	for(char test = '\0'; test!=ENTER_KEY ; )
		test = (char) wgetch(menu);

	startGame(menu, map);
}

/**
 * @brief tells the player how to control this game
 * @param menu pointer to the menu window 
 */
void controls(WINDOW * menu){
//todo just write all controls
}

/**
 * @brief erases all memory from the menu window and closes it
 * @param menu pointer to the menu window that is supposed to be closed
 */
void closeMenu(WINDOW * menu){
	werase(menu);
	wrefresh(menu);
	delwin(menu);
}


#endif //SEMESTRALKA_MENU_H

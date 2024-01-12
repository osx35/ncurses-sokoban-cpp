#include <iostream>
#include <fstream>
#include <ncursesw/curses.h>
#include "mapa.h"

bool canMove(int newRow, int newCol, Map &m, char dir, int boxbox);
bool moveBox(int boxRow, int boxCol, char dir, Map &m);

void ncurses_init_colors() {
	// Więcej o kolorach tu https://www.linuxjournal.com/content/programming-color-ncurses

	// musimy ustawic jesli bedziemy uzywac kolorowania konsoli
	start_color();
	// i zdefiniowac pary kolorow ktore bedziemy uzywac
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
}

void ncurses_config() {
	// definiujemy kolory tekstu ktore uzyjemy do kolorowania planszy
	ncurses_init_colors();
	// getch ma interpretowac znaki specjalne takie jak KEY_UP
	keypad(stdscr, TRUE);
	// ustawiamy timeout dla getch (milisekundy)
	//po tym czasie program przejdzie dalej nawet jesli nie podasz klawisza
	timeout(30);
	// podczas wpisywania z klawiatury nie powinna sie drukowac literka
	noecho();
	// ukrycie kursora
	curs_set(0);
}

void print_board(int x, int y, int character, Map &m, int screenH, int screenW) {
	clear();

	attron(COLOR_PAIR(2));
	m.printMapNCurses(screenH, screenW);
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(1));
	move(y + screenH, x + screenW);
	addch(character);
	attroff(COLOR_PAIR(1));


	mvprintw(1, 0, "Level: (%d), Map Dimensions: (%d, %d)", m.level, m.cols, m.rows);
	mvprintw(2, 0, "Player Position: (%d, %d)", x, y);
	mvprintw(3, 0, "Box count: (%d), Goal count: (%d)", m.box.count, m.goal.count);

	int boxCount = 0;
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			if (m.mapaArray[i][j] == '$') {
				mvprintw(5 + boxCount, 0, "Box Position: (%d, %d)", j, i);
				boxCount++;
			}
		}
	}


	int goalCount = 0;
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			if (m.originalArray[i][j] == '.') {
				mvprintw(5+m.rows + goalCount, 0, "Goal Position: (%d, %d)", j, i);
				goalCount++;
			}
		}
	}

	refresh();
}

int main(void) {
	Map mapa1;
	mapa1.pathName = "mapy/mapa.txt";
	mapa1.initLevel();

	// inicjalizacja ncurses
	WINDOW *mainwin = initscr();
	ncurses_config();

	int screenHeight=0, screenWidth=0;
	// pobiera wymiary terminala
	getmaxyx(stdscr, screenHeight, screenWidth);

	int last_character = '@';
	int fixedWidth = (screenWidth - mapa1.cols) / 2;
	int fixedHeight = (screenHeight - mapa1.rows) / 2;

	int last_position_x = mapa1.player.col;
	int last_position_y = mapa1.player.row;


	bool playing = true;
	// glowna nieskonczona petla programu
	while (playing) {
		if (mapa1.allGoalsReached()) {
			mapa1.level++;
			switch (mapa1.level) {
				case 1:
					mapa1.pathName = "mapy/mapa1.txt";
					mapa1.initLevel();
					last_position_x = mapa1.player.col;
					last_position_y = mapa1.player.row;
					break;
				case 2:
					mapa1.pathName = "mapy/mapa2.txt";
					mapa1.initLevel();
					last_position_x = mapa1.player.col;
					last_position_y = mapa1.player.row;
					break;
				case 3:
					mapa1.pathName = "mapy/mapa3.txt";
					mapa1.initLevel();
					last_position_x = mapa1.player.col;
					last_position_y = mapa1.player.row;
					break;
				case 4:
					mapa1.pathName = "mapy/mapa4.txt";
					mapa1.initLevel();
					last_position_x = mapa1.player.col;
					last_position_y = mapa1.player.row;
					break;
				case 5:
					mapa1.pathName = "mapy/mapa5.txt";
					mapa1.initLevel();
					last_position_x = mapa1.player.col;
					last_position_y = mapa1.player.row;
					break;
			}
		}
		getmaxyx(stdscr, screenHeight, screenWidth);
		int fixedWidth = (screenWidth - mapa1.cols) / 2;
		int fixedHeight = (screenHeight - mapa1.rows) / 2;
		int new_position_x = last_position_x;
		int new_position_y = last_position_y;
		char direction;
		int input = getch();
		if (input != ERR) {
			switch (input) {
				case KEY_UP:
					if (last_position_y > 0) {
						--new_position_y;
						direction = 'u';
						if (canMove(new_position_y, last_position_x, mapa1, direction, 0)) {
							last_position_y = new_position_y;
							if (mapa1.mapaArray[last_position_y][last_position_x] == '$') {
								moveBox(last_position_y, last_position_x, direction, mapa1);
							}
						}
					}
					break;
				case KEY_DOWN:
					if (last_position_y < mapa1.rows - 1) {
						++new_position_y;
						direction = 'd';
						if (canMove(new_position_y, last_position_x, mapa1, direction, 0)) {
							last_position_y = new_position_y;
							if (mapa1.mapaArray[last_position_y][last_position_x] == '$') {
								moveBox(last_position_y, last_position_x, direction, mapa1);
							}
						}
					}
					break;
				case KEY_LEFT:
					if (last_position_x > 0) {
						--new_position_x;
						direction = 'l';
						if (canMove(last_position_y, new_position_x, mapa1, direction, 0)) {
							last_position_x = new_position_x;
							if (mapa1.mapaArray[last_position_y][last_position_x] == '$') {
								moveBox(last_position_y, last_position_x, direction, mapa1);
							}
						}
					}
					break;
				case KEY_RIGHT:
					if (last_position_x < mapa1.cols - 1) {
						++new_position_x;
						direction = 'r';
						if (canMove(last_position_y, new_position_x, mapa1, direction, 0)) {
							last_position_x = new_position_x;
							if (mapa1.mapaArray[last_position_y][last_position_x] == '$') {
								moveBox(last_position_y, last_position_x, direction, mapa1);
							}
						}
					}
					break;
				case 'r':
				case 'R':
					mapa1.resetLevel();
					last_position_x = mapa1.player.col;
					last_position_y = mapa1.player.row;
					break;
				case 'q':
				case 'Q':
				case 27:
					// wychodzimy z programu
					playing = false;
					break;
			}

		}
		// nasza funkcja ma wyswietlac cos na ekranie na podstawie przekazanych danych
		print_board(last_position_x, last_position_y, last_character, mapa1, fixedHeight, fixedWidth);
	}

	// zakanczamy prace ncurses
	delwin(mainwin);
	endwin();
	refresh();
	return EXIT_SUCCESS;
}

bool canMove(int newRow, int newCol, Map &m, char dir, int boxbox) {

	if (newRow < 0 || newRow >= m.rows || newCol < 0 || newCol >= m.cols) {
		return false;
	}

	if (m.mapaArray[newRow][newCol] == '#') {
		return false;
	}

	if (m.mapaArray[newRow][newCol] == '$') {
		if(boxbox >0)
			return false;
		switch(dir) {
			case 'u':
				return canMove(newRow-1, newCol,m,dir, boxbox+1);
				break;
			case 'd':
				return canMove(newRow+1, newCol,m,dir,boxbox+1);
				break;
			case 'l':
				return canMove(newRow, newCol-1,m,dir,boxbox+1);
				break;
			case 'r':
				return canMove(newRow, newCol+1,m,dir, boxbox+1);
				break;
		}
	}

	return true;
}

bool moveBox(int boxRow, int boxCol, char dir, Map &m) {
	int newRow = boxRow;
	int newCol = boxCol;

	switch (dir) {
		case 'u':
			newRow--;
			break;
		case 'd':
			newRow++;
			break;
		case 'l':
			newCol--;
			break;
		case 'r':
			newCol++;
			break;
	}

	if (newRow < 0 || newRow >= m.rows || newCol < 0 || newCol >= m.cols) {
		return false;
	}


	if (m.mapaArray[newRow][newCol] == ' ' || m.mapaArray[newRow][newCol] == '.') {

		m.mapaArray[newRow][newCol] = '$';
		if(m.originalArray[boxRow][boxCol]=='.')
			m.mapaArray[boxRow][boxCol] = '.';
		else
			m.mapaArray[boxRow][boxCol] = ' ';
		return true;
	}

	return false;
}
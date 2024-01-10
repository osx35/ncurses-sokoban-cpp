#include <iostream>
#include <fstream>
#include <ncursesw/curses.h>
#include "mapa.h"

bool canMove(int newRow, int newCol, Map &m, char dir, int boxbox);

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
	// operujemy na wirtualnym ekranie bedacym buforem ekranu
	// a nastepnie wyswietlamy bufor w terminalu funckja refresh

	clear(); // czysci wirtualny ekran (lepiej bylo by czyscic jeden znak albo jedna linie)

	// ustawiamy wczesniej zdefiniowane pary kolorow
	attron(COLOR_PAIR(2));
	m.printMapNCurses(screenH, screenW);
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(1));
	// szkaczemy kursorem do danej pozycji
	move(y + screenH, x + screenW);
	addch(character);

	// przywracamy domyslny kolor
	attroff(COLOR_PAIR(1));

	mvprintw(0, 0, "Player Position: (%d, %d), Map Dimensions: (%d, %d)", x, y, m.cols, m.rows);
	mvprintw(1, 0, "Box Position and count: (%d, %d, %d), Goal Position and count: (%d, %d, %d)", m.box.col, m.box.row,m.box.count, m.goal.col, m.goal.row, m.goal.count);
	mvprintw(2, 0, "Level: (%d)",m.level );
	// wyswietlamy zawartosc wirtualnego ekranu dopiero po refresh
	refresh();
}

int main(void) {
	Map mapa1;
	mapa1.pathName = "mapa.txt";
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
		getmaxyx(stdscr, screenHeight, screenWidth);
		int fixedWidth = (screenWidth - mapa1.cols) / 2;
		int fixedHeight = (screenHeight - mapa1.rows) / 2;
		int new_position_x = last_position_x;
		int new_position_y = last_position_y;
		char direction;
		// szczytujemy literke z klawiatury
		// (jesli sa tu znaki specjlane musi byc int bo nie zmieszcza sie w char)
		int input = getch();
		if (input != ERR) {
			switch (input) {
				case KEY_UP:
					// sprawdzamy czy nie wychodzimy poza ekran
					if (last_position_y > 0) {
						--new_position_y;
						direction = 'u';
						if(canMove(new_position_y, last_position_x, mapa1, direction, 0))
							last_position_y = new_position_y;

					}
					break;
				case KEY_DOWN:
					if (last_position_y < mapa1.rows - 1) {
						++new_position_y;
						direction = 'd';
						if(canMove(new_position_y, last_position_x, mapa1, direction, 0))
							last_position_y = new_position_y;

					}
					break;
				case KEY_LEFT:
					if (last_position_x > 0) {
						--new_position_x;
						direction = 'l';
						if(canMove(last_position_y, new_position_x, mapa1, direction, 0))
							last_position_x = new_position_x;

					}
					break;
				case KEY_RIGHT:
					if (last_position_x < mapa1.cols - 1) {
						++new_position_x;
						direction = 'r';
						if(canMove(last_position_y, new_position_x, mapa1, direction, 0))
							last_position_x = new_position_x;

					}
					break;
				case 'r':
				case 'R':
					mapa1.resetLevel();
					last_position_x = mapa1.player.col;
					last_position_y = mapa1.player.row;
					break;
				case 't':
					mapa1.level=1;
					mapa1.pathName = "mapa1.txt";
					mapa1.initLevel();
					break;
				case 'T':
					mapa1.level =0;
					mapa1.pathName = "mapa.txt";
					mapa1.initLevel();
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
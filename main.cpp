#include <iostream>
#include <fstream>
#include <ncursesw/curses.h>
#include "mapa.h"


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

void print_board(int x, int y, char playerCharacter, Map &m, Position fixed) {
	clear();

	attron(COLOR_PAIR(2));
	m.printMapNCurses(fixed.col, fixed.row);
	attroff(COLOR_PAIR(2));

	attron(COLOR_PAIR(1));
	move(y + fixed.col, x + fixed.row);
	addch(playerCharacter);
	attroff(COLOR_PAIR(1));


	mvprintw(1, 0, "Level: (%d), Map Dimensions: (%d, %d)", m.level, m.cols, m.rows);
	mvprintw(2, 0, "Player Position: (%d, %d)", x, y);
	mvprintw(3, 0, "Box count: (%d), Goal count: (%d)", m.box.count, m.goal.count);

	refresh();
}

int main(void) {

	Map mapa1;
	mapa1.pathName = "mapy/mapa.txt";
	mapa1.initLevel();

	// inicjalizacja ncurses
	WINDOW *mainwin = initscr();
	ncurses_config();

	Position screen;
	// pobiera wymiary terminala
	getmaxyx(stdscr, screen.col, screen.row);

	Position fixed;
	fixed.col = (screen.col - mapa1.cols) / 2;
	fixed.row = (screen.row - mapa1.rows) / 2;

	Position last;
	last.col = mapa1.player.col;
	last.row = mapa1.player.row;

	bool playing = true;
	// glowna nieskonczona petla programu
	while (playing) {
		
		if (mapa1.allGoalsReached()) {
			mapa1.advanceToNextLevel();
			last.col = mapa1.player.col;
			last.row = mapa1.player.row;
			fixed.col = (screen.col - mapa1.cols) / 2;
			fixed.row = (screen.row - mapa1.rows) / 2;
		}

		Position neww;
		neww.col = last.col;
		neww.row = last.row;

		char direction;
		int input = getch();

		if (input != ERR) {
			switch (input) {
				case KEY_UP:
					if (last.row > 0) {
						mapa1.movePlayer('u', --neww.row,neww.col, last);
					}
					break;
				case KEY_DOWN:
					if (last.row < mapa1.rows - 1) {
						mapa1.movePlayer('d', ++neww.row, neww.col, last);
					}
					break;
				case KEY_LEFT:
					if (last.col > 0) {
						mapa1.movePlayer('l', neww.row, --neww.col, last);
					}
					break;
				case KEY_RIGHT:
					if (last.col < mapa1.cols - 1) {
						mapa1.movePlayer('r', neww.row, ++neww.col, last);
					}
					break;
				case 'r':
				case 'R':
					mapa1.resetLevel();
					last.col = mapa1.player.col;
					last.row = mapa1.player.row;
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
		print_board(last.col, last.row, '@', mapa1, fixed);
	}

	// zakanczamy prace ncurses
	delwin(mainwin);
	endwin();
	refresh();
	return EXIT_SUCCESS;
}



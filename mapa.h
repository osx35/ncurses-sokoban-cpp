#include <iostream>
#include <fstream>
#include <vector>

#ifndef MAPA_H
#define MAPA_H

class Position {
	public:
		int row;
		int col;
		int count = 0;
};

class Map {
	public:
		int level = 0;
		std::string pathName;
		std::vector<std::vector<char>> mapaArray;
		std::vector<std::vector<char>> originalArray;
		int rows;
		int cols;
		Position player;
		Position box;
		Position goal;

		void sprawdzRozmiar() {
			std::fstream mapa;
			mapa.open(pathName, std::ios::in);

			if (!mapa.good()) {
				std::cout << "Nie ma takiego pliku";
				return;
			}

			rows = 0;
			cols = 0;

			std::string linia;
			while (getline(mapa, linia)) {
				++rows;
				if (rows == 1) {
					cols = linia.size();
				}
			}

			mapa.close();
		}

		void itemsLocation() {
			std::fstream mapa;
			mapa.open(pathName, std::ios::in);

			if (!mapa.good()) {
				std::cout << "Nie ma takiego pliku";
				return;
			}

			mapaArray.resize(rows, std::vector<char>(cols));
			originalArray.resize(rows, std::vector<char>(cols));

			std::string linia;
			for (int i = 0; i < rows; ++i) {
				getline(mapa, linia);

				for (int j = 0; j < cols; ++j) {
					mapaArray[i][j] = linia[j];
					originalArray[i][j] = linia[j];
					char current = mapaArray[i][j];
					switch (current) {
						case '@':
							mapaArray[i][j] = ' ';
							player.row = i;
							player.col = j;
							break;
						case '$':
							box.count++;
							break;
						case '.':
							goal.count++;
							break;
					}
				}
			}
			mapa.close();
		}

		void printMapNCurses(int strow, int stcol) {
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					mvprintw(strow + i, stcol + j, "%c", mapaArray[i][j]);
				}
			}
		}

		void resetLevel() {
			mapaArray = originalArray;
			initLevel();
		}

		void initLevel() {
			box.count =0;
			goal.count =0;
			sprawdzRozmiar();
			itemsLocation();

		}

		bool allGoalsReached() {
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					if (originalArray[i][j] == '.' && mapaArray[i][j] != '$') {
						return false;
					}
				}
			}
			return true;
		}

		void advanceToNextLevel() {
			level++;
			pathName = "mapy/mapa" + std::to_string(level) + ".txt";
			initLevel();
		}

		bool canMove(int newRow, int newCol, char dir, int boxbox) {
			if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) {
				return false;
			}

			if (mapaArray[newRow][newCol] == '#') {
				return false;
			}

			if (mapaArray[newRow][newCol] == '$') {
				if (boxbox > 0) {
					return false;
				}

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

				return canMove(newRow, newCol, dir, boxbox + 1);
			}

			return true;
		}

		bool moveBox(int boxRow, int boxCol, char dir) {
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

			if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) {
				return false;
			}

			if (mapaArray[newRow][newCol] == ' ' || mapaArray[newRow][newCol] == '.') {

				mapaArray[newRow][newCol] = '$';
				if (originalArray[boxRow][boxCol] == '.')
					mapaArray[boxRow][boxCol] = '.';
				else
					mapaArray[boxRow][boxCol] = ' ';
				return true;
			}

			return false;
		}

		void movePlayer(char direction, int &roww, int &coll, Position &last) {

			if (canMove(roww, coll, direction, 0)) {
				last.row = roww;
				last.col =coll;
				if (mapaArray[last.row][last.col] == '$') {
					moveBox(last.row, last.col, direction);
				}
			}

		}
};

#endif
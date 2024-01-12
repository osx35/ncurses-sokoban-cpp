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
		bool allGoalsReached() const {
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j) {
					if (originalArray[i][j] == '.' && mapaArray[i][j] != '$') {
						return false;
					}
				}
			}
			return true;
		}
};

#endif
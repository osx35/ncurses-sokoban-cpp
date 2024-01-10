#include <iostream>
#include <fstream>
#include <ncursesw/curses.h>
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
    std::vector<std::vector<char>> mapaArray;
    std::vector<std::vector<char>> originalArray; 
    int rows;
    int cols;
    Position player;
    Position box;
    Position goal;
    bool boxOnGoal;

    void sprawdzRozmiar() {
        std::fstream mapa;
        mapa.open("mapa.txt", std::ios::in);

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
        mapa.open("mapa.txt", std::ios::in);

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
                        player.row = i;
                        player.col = j;
                        break;
                    case '$':
                        box.row = i;
                        box.col = j;
                        box.count++;
                        break;
                    case '.':
                        goal.row = i;
                        goal.col = j;
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
                if (mapaArray[i][j] != '@') {
                    mvprintw(strow + i, stcol + j, "%c", mapaArray[i][j]);
                }
            }
        }
    }

    // Function to reset the level by copying originalArray back to mapaArray
    void resetLevel() {
        mapaArray = originalArray;
    }
};

#endif
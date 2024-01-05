#include <iostream>
#include <fstream>

#ifndef MAPA_H
#define MAPA_H

class Map {
private:
    int rows;
    int cols;

public:
    Map() : rows(0), cols(0) {}

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

    void printMap() {
        std::fstream mapa;
        mapa.open("mapa.txt", std::ios::in);
        std::string linia;
        char mapaArray[rows][cols];

        for (int i = 0; i < rows; ++i) {
            getline(mapa, linia);

            for (int j = 0; j < cols; ++j) {
                mapaArray[i][j] = linia[j];
            }
        }

        mapa.close();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << mapaArray[i][j];
            }
            std::cout << '\n';
        }
    }
};
#endif
#include <iostream>
#include <fstream>

#ifndef MAPA_H
#define MAPA_H

struct MapSize {
    int rows;
    int cols;
};

MapSize sprawdzRozmiar() {
    std::fstream mapa;
    mapa.open("mapa.txt", std::ios::in);

    if (!mapa.good()) {
        std::cout << "Nie ma takiego pliku";
        return {0, 0};  
    }

    MapSize size;
    size.rows = 0;
    size.cols = 0;

    std::string linia;
    while (getline(mapa, linia)) {
        ++size.rows;
        if (size.rows == 1) {
            size.cols = linia.size();
        }
    }

    mapa.close();
    return size;
}

void printMap(int rows, int cols) {
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
#endif 

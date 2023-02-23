#ifndef WORDLE_ART_H
#define WORDLE_ART_H

#include <windows.h> 
#include <iostream>
#include <vector>

enum color {
    green=10,
    yellow=14,
    gray=8,
    white=15
};

/**
 * Returns a vector of colors with given size
 * Default initialized to white
*/
std::vector<int> defaultColors (int size) {
    std::vector<int> v;
    for (int i = 0; i < size; i++) {
        v.push_back(white);
    }
    return v;
}

/**
 * Print a "line" of letters in wordle syntax
 * Each with individual color, of form:
 *  ---
 * | Q |
 *  ---
 * Precondition: letters.size() == colors.size()
*/
void fancyLine(std::string letters, std::vector<int> colors) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (unsigned i = 0; i < letters.size(); i++) {
        int currColor = colors[i];
        SetConsoleTextAttribute(hConsole, currColor);
        std::cout << " --- ";
    }
    std::cout << std::endl;
    for (unsigned i = 0; i < letters.size(); i++) {
        int currColor = colors[i];
        SetConsoleTextAttribute(hConsole, currColor);
        std::cout << "| " << letters[i] << " |";
    }
    std::cout << std::endl;
    for (unsigned i = 0; i < letters.size(); i++) {
        int currColor = colors[i];
        SetConsoleTextAttribute(hConsole, currColor);
        std::cout << " --- ";
    }
    std::cout << std::endl;
}

void howToPlay() {
    int lineLength = 25;
    std::cout << "=============================================" << std::endl;

}

#endif
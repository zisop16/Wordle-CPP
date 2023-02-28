#ifndef WORDLE_ART_H
#define WORDLE_ART_H

#include <windows.h>
#include <iostream>
#include <vector>

enum color
{
    green = 10,
    yellow = 14,
    gray = 8,
    grey = 8,
    white = 15
};

void clearScreen()
{
    std::cout << "\x1B[2J\x1B[H";
}

std::string toUpper(std::string s)
{
    for (unsigned int i = 0; i < s.size(); i++)
    {
        char curr = s[i];
        if (curr >= 'a' && curr <= 'z')
        {
            s[i] -= 0x20;
        }
    }
    return s;
}

/**
 * Returns a vector of colors with given size
 * Default initialized to white
 */
std::vector<int> defaultColors(int size)
{
    std::vector<int> v;
    for (int i = 0; i < size; i++)
    {
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
void fancyLine(std::string letters, std::vector<int> colors, int space=0)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    letters = toUpper(letters);
    std::cout << std::string(space, ' ');
    for (unsigned i = 0; i < letters.size(); i++)
    {
        int currColor = colors[i];
        SetConsoleTextAttribute(hConsole, currColor);
        std::cout << " --- ";
    }
    std::cout << std::endl;
    std::cout << std::string(space, ' ');
    for (unsigned i = 0; i < letters.size(); i++)
    {
        int currColor = colors[i];
        SetConsoleTextAttribute(hConsole, currColor);
        char curr = letters[i];
        std::cout << "| " << curr << " |";
    }
    std::cout << std::endl;
    std::cout << std::string(space, ' ');
    for (unsigned i = 0; i < letters.size(); i++)
    {
        int currColor = colors[i];
        SetConsoleTextAttribute(hConsole, currColor);
        std::cout << " --- ";
    }
    SetConsoleTextAttribute(hConsole, white);
    std::cout << std::endl;
}

void howToPlay()
{
    std::cout << "=============================================" << std::endl;
    std::cout << "                 HOW TO PLAY" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Guess the Wordle in 6 tries.\n\n"
              << std::endl;

    std::cout << "HOW TO PLAY:" << std::endl;
    std::cout << "- Each guess must be a valid 5 letter word." << std::endl;
    std::cout << "- The color of the tiles will change to show" << std::endl;
    std::cout << "  you how close your guess was to the word.\n\n"
              << std::endl;

    std::vector<int> colors = defaultColors(5);
    colors[0] = green;
    std::string letters = "WEARY";
    fancyLine(letters, colors);
    std::cout << "W is in the word and in the correct spot.\n"
              << std::endl;
    colors[0] = white;
    colors[1] = yellow;
    letters = "PILLS";
    fancyLine(letters, colors);
    std::cout << "I is in the word but in the wrong spot.\n"
              << std::endl;
    colors[1] = white;
    colors[3] = grey;
    letters = "VAGUE";
    fancyLine(letters, colors);
    std::cout << "U is not in the word in any spot.\n\n"
              << std::endl;
}

void menuText()
{
    std::cout << "=========================" << std::endl;
    std::cout << "    WELCOME TO WORDLE" << std::endl;
    std::cout << "=========================\n\n"
              << std::endl;
    std::cout << "1. Play Wordle" << std::endl;
    std::cout << "2. How to Play" << std::endl;
    std::cout << "3. Statistics Summary" << std::endl;
    std::cout << "4. Reset Statistics" << std::endl;
    std::cout << "5. Exit\n\n"
              << std::endl;
    std::cout << "Select an option: ";
}

#endif
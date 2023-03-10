#ifndef WORDLE_ART_H
#define WORDLE_ART_H

#include <iostream>
#include <vector>

#define green "\e[0;32m"
#define grey "\033[38;5;245m"
#define gray grey
#define yellow "\e[0;33m"
#define white "\e[0;37m"
#define resetColors "\e[0m"
#define clearScreen "\x1B[2J\x1B[H"

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
std::vector<std::string> defaultColors(int size)
{
    std::vector<std::string> v;
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
void fancyLine(std::string letters, std::vector<std::string> colors, int space = 0)
{
    letters = toUpper(letters);
    std::cout << std::string(space, ' ');
    for (unsigned i = 0; i < letters.size(); i++)
    {
        std::string currColor = colors[i];
        std::cout << currColor << " --- ";
    }
    std::cout << std::endl;
    std::cout << std::string(space, ' ');
    for (unsigned i = 0; i < letters.size(); i++)
    {
        std::string currColor = colors[i];
        char curr = letters[i];
        std::cout << currColor << "| " << curr << " |";
    }
    std::cout << std::endl;
    std::cout << std::string(space, ' ');
    for (unsigned i = 0; i < letters.size(); i++)
    {
        std::string currColor = colors[i];
        std::cout << currColor << " --- ";
    }
    std::cout << resetColors << std::endl;
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

    std::vector<std::string> colors = defaultColors(5);
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
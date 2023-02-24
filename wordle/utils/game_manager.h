#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include "wordle_art.h"
#include <stdlib.h>
#include <fstream>
#include <random>
#include <vector>

static std::vector<std::string> words;
static std::vector<std::string> allowed;

enum options
{
    play = 1,
    guide = 2,
    stats = 3,
    reset = 4,
    quit = 5
};

void pressEnter()
{
    std::cout << "Press [enter] to continue";
    std::cin.ignore();
}

std::vector<std::string> readLines(std::string filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (file.is_open())
    {
        while (file)
        {
            std::string currLine;
            file >> currLine;
            lines.push_back(currLine);
        }
    }
    return lines;
}

void initializeWords()
{
    words = readLines("data/words.txt");
    allowed = readLines("data/allowed.txt");
}

std::vector<int> guessColors(std::string guess, std::string correctWord)
{
    std::vector<int> colors = defaultColors(5);
    for (unsigned int i = 0; i < 5; i++)
    {
        char curr = guess[i];
        if (correctWord.find(curr) == std::string::npos)
        {
            colors[i] = gray;
        } // else if (correctWord[i] )
    }
    return colors;
}

void playGame()
{
    if (words.size() == 0)
    {
        initializeWords();
    }
    int max = words.size();
    int randIndex = std::rand() % max;
    std::string correctWord = words[randIndex];
    std::cout << "Correct Word: " << correctWord << std::endl;

    int maxGuesses = 6;
    for (int guessNum = 0; guessNum < maxGuesses; guessNum++)
    {
        std::cout << "Enter guess: ";
        std::string guess;
        std::cin >> guess;
        std::cin.ignore();

        std::vector<int> colors = guessColors(guess, correctWord);
        // std::system("CLS");
        fancyLine(guess, colors);
    }
}

int respondToOption()
{
    int option;
    std::cin >> option;
    std::cin.ignore();
    // std::system("CLS");
    switch (option)
    {
    case play:
        playGame();
        break;
    case guide:
        howToPlay();
        break;
    case stats:
        break;
    case reset:
        break;
    case quit:
        break;
    }
    return option;
}

void runGame()
{
    while (true)
    {
        // std::system("CLS");
        menuText();
        int option = respondToOption();
        if (option == quit)
        {
            break;
        }
        pressEnter();
    }
}

#endif
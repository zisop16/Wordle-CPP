#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include "wordle_art.h"
#include <stdlib.h>
#include <fstream>
#include <random>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>

static std::vector<std::string> words;
static std::unordered_set<std::string> allowed;

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
    std::vector<std::string> allowedWords = readLines("data/allowed.txt");
    // Supposedly this line copies the vector into the set
    std::copy(allowedWords.begin(), allowedWords.end(), std::inserter(allowed, allowed.end()));
    for (unsigned int i = 0; i < words.size(); i++)
    {
        allowed.insert(words.at(i));
    }
}

std::vector<int> guessColors(std::string guess, std::string correctWord)
{
    std::vector<int> colors = defaultColors(5);
    /* Map from char -> vector of indices for guess
     * Ex: guess = "hello"
     * characterIndices = {
     *   'h': {0},
     *   'e': {1},
     *   'l': {2, 3},
     *   'o': {4}
     * }
     */
    // Have to use an ordered set so that indices are iterated over from smallest to largest
    std::unordered_map<char, std::set<int>> guessIndicesMap;
    std::unordered_map<char, std::set<int>> correctIndicesMap;
    for (unsigned int i = 0; i < 5; i++)
    {
        char guessChar = guess[i];
        char correctChar = correctWord[i];
        // This apparently implicitly creates a vector at the key if one doesn't already exist,
        // And sets guessIndices to a reference to that vector
        std::set<int> &guessIndices = guessIndicesMap[guessChar];
        guessIndices.insert(i);
        std::set<int> &correctIndices = correctIndicesMap[correctChar];
        correctIndices.insert(i);
    }
    // Stupid fucking shitty algorithm to color code guess
    for (std::pair<char, std::set<int>> const &pair : guessIndicesMap)
    {
        char currChar = pair.first;
        std::set<int> guessIndices = pair.second;
        std::set<int> correctIndices = correctIndicesMap[currChar];
        int allowedYellow = (int)(correctIndices.size());
        std::set<int> alreadyChecked;
        for (int const &ind : correctIndices)
        {
            if (guessIndices.count(ind))
            {
                colors[ind] = green;
                allowedYellow -= 1;
                alreadyChecked.insert(ind);
            }
        }
        for (int const &ind : guessIndices)
        {
            if (allowedYellow == 0)
            {
                break;
            }
            if (alreadyChecked.count(ind))
            {
                continue;
            }
            colors[ind] = yellow;
            allowedYellow -= 1;
        }
    }
    for (unsigned int i = 0; i < colors.size(); i++)
    {
        if (colors[i] == white)
        {
            colors[i] = grey;
        }
    }
    return colors;
}

std::string getGuess()
{
    std::string guess;
    bool firstIteration = true;
    do
    {
        if (!firstIteration)
        {
            std::cout << "Only 5 letter english words are allowed!" << std::endl;
        }
        std::cout << (firstIteration ? "Enter guess: " : "Enter a different guess: ");
        std::cin >> guess;
        std::cin.ignore();
        firstIteration = false;
        // If guess is not contained in allowed, keep looping
    } while (!allowed.count(guess));
    return guess;
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
    std::vector<std::string> pastGuesses;
    std::vector<std::vector<int>> pastColors;
    for (int guessNum = 0; guessNum < maxGuesses; guessNum++)
    {
        std::string guess = getGuess();
        std::vector<int> colors = guessColors(guess, correctWord);
        pastGuesses.push_back(guess);
        pastColors.push_back(colors);
        std::system("CLS");
        for (unsigned int i = 0; i < pastGuesses.size(); i++)
        {
            std::string currGuess = pastGuesses.at(i);
            std::vector<int> currColors = pastColors.at(i);
            fancyLine(currGuess, currColors);
        }
    }
}

int respondToOption()
{
    int option;
    std::cin >> option;
    std::cin.ignore();
    std::system("CLS");
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
        std::system("CLS");
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
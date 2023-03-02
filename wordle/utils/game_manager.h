#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include "../../wordle_art.h"
#include "stats_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fstream>
#include <random>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <filesystem>

static std::vector<std::string> words;
static std::unordered_set<std::string> allowed;

enum options
{
    play = 1,
    guide = 2,
    stats = 3,
    userReset = 4,
    quit = 5
};

void delete_dir_content(std::string dir_path)
{
    for (auto &path : std::filesystem::directory_iterator(dir_path))
    {
        std::filesystem::remove_all(path);
    }
}

void generateToken(std::string token)
{
    const std::string tokenPath = "../keyboard/tokens";
    if (!std::filesystem::exists(tokenPath))
    {
        std::filesystem::create_directory(tokenPath);
    }
    delete_dir_content(tokenPath);
    std::ofstream output(tokenPath + "/" + token);
}

void pressEnter()
{
    std::cout << "Press [enter] to continue";
    std::cin.ignore();
}

static std::string colorFilePath = "../keyboard/color.txt";

void writeColors(std::string letters, std::vector<std::string> colors) {
    for (int i = 0; i < letters.size(); i++) {
        if ('a' <= letters[i] && 'z' >= letters[i]) {
            letters[i] += 'A' - 'a';
        }
    }
    std::ofstream colorFile(colorFilePath);
    if (colorFile.is_open()) {
        for (unsigned int i = 0; i < colors.size(); i++) {
            std::string currLine = letters[i] + colors[i];
            colorFile << currLine;
            if (i != colors.size() - 1) {
                colorFile << "\n";
            }
        }
    }
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

std::vector<std::string> guessColors(std::string guess, std::string correctWord)
{
    std::vector<std::string> colors = defaultColors(5);
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
    generateToken("gameStart");
    if (words.size() == 0)
    {
        initializeWords();
    }
    int max = words.size();
    std::srand(std::time(nullptr));
    int randIndex = std::rand() % max;
    std::string correctWord = words[randIndex];
    // std::cout << "Correct Word: " << correctWord << std::endl;

    int maxGuesses = 6;
    std::vector<std::string> pastGuesses;
    std::vector<std::vector<std::string>> pastColors;
    bool foundSolution = false;
    int attempts = 0;
    for (int guessNum = 0; guessNum < maxGuesses; guessNum++)
    {
        attempts += 1;
        std::string guess = getGuess();
        std::vector<std::string> colors = guessColors(guess, correctWord);
        pastGuesses.push_back(guess);
        pastColors.push_back(colors);
        std::cout << clearScreen;
        for (unsigned int i = 0; i < pastGuesses.size(); i++)
        {
            std::string currGuess = pastGuesses.at(i);
            std::vector<std::string> currColors = pastColors.at(i);
            fancyLine(currGuess, currColors);
        }
        writeColors(guess, colors);
        generateToken("updateColors");
        if (guess == correctWord)
        {
            foundSolution = true;
            break;
        }
    }
    if (foundSolution)
    {
        std::cout << "\nSplendid!\n"
                  << std::endl;
    }
    else
    {
        std::cout << "\nThe word was: " << toUpper(correctWord) << "\n\nBetter luck next time!\n"
                  << std::endl;
    }
    addToStats(correctWord, attempts, foundSolution);
    generateToken("gameLaunch");
}

void onExit() {
    generateToken("gameExit");
    if (std::filesystem::exists(colorFilePath)) {
        std::remove(colorFilePath.c_str());
    }
}


int respondToOption()
{
    std::string input;
    std::cin >> input;
    std::cin.ignore();
    if (input.size() != 1 || !(input[0] >= '0' && input[1] <= '5'))
    {
        std::cout << "Please enter one of the given options" << std::endl;
        return respondToOption();
    }
    // '1' - '0' == 1
    int option = input[0] - '0';
    std::cout << clearScreen;
    switch (option)
    {
    case play:
        playGame();
        break;
    case guide:
        howToPlay();
        break;
    case stats:
        showStats();
        break;
    case userReset:
        resetStats();
        break;
    case quit:
        onExit();
        break;
    }
    return option;
}

void sigintHandler(int sig_num)
{
    onExit();
    exit(sig_num);
}

void runGame()
{
    signal(SIGINT, sigintHandler);
    generateToken("gameLaunch");
    while (true)
    {
        std::cout << clearScreen;
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
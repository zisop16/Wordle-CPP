#ifndef STATS_MANAGER_H
#define STATS_MANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "../../wordle_art.h"
#include <math.h>
#include <iomanip>
#include <cstring>

static std::string statsFile = "data/history.txt";

bool isEmpty(std::string fileName)
{
    std::ifstream file(fileName);
    if (file)
    {
        return (file.get(), file.eof());
    }
    return true;
}

void addToStats(std::string correctWord, int attempts, bool foundSolution)
{
    std::ofstream file(statsFile, std::ios_base::app);
    if (file.is_open())
    {
        if (!isEmpty(statsFile))
        {
            file << '\n';
        }
        file << std::to_string(correctWord.size()) << correctWord << std::to_string(attempts) << std::to_string(foundSolution ? 1 : 0);
    }
}

void showStats()
{
    std::ifstream file(statsFile);
    int played = 0;
    int totalAttempts = 0;
    int wins = 0;
    int currStreak = 0;
    int maxStreak = 0;
    std::vector<std::string> games;
    if (file.is_open())
    {
        while (file)
        {
            std::string currLine;
            // Format of currline:
            // nw*nii
            // If n=6, then example:
            // 6losers11
            std::getline(file, currLine);
            std::cout << currLine << std::endl;
            if (currLine.size() == 0) {
                continue;
            }
            int wordLength = currLine[0] - '0';
            if (currLine.size() != wordLength + 3)
            {
                continue;
            }
            games.push_back(currLine);
            std::string word = currLine.substr(1, wordLength);
            int attempts = std::stoi(currLine.substr(wordLength + 1, 1));
            bool foundSolution = (bool)(std::stoi(currLine.substr(wordLength + 2, 1)));
            played += 1;
            if (foundSolution)
            {
                wins += 1;
                totalAttempts += attempts;
                currStreak += 1;
            }
            else
            {
                if (currStreak > maxStreak)
                {
                    maxStreak = currStreak;
                }
                currStreak = 0;
            }
        }
    }
    if (currStreak > maxStreak)
    {
        maxStreak = currStreak;
    }

    std::cout << "==========================" << std::endl;
    std::cout << "    STATISTICS SUMMARY" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Times Played:";
    int space = 13;
    std::string temp = std::to_string(played);
    space -= temp.size();
    std::cout << std::string(space, ' ') << temp << std::endl;
    std::cout << "Average Attempts:";
    float avg;
    if (wins == 0)
    {
        avg = 0;
    }
    else
    {
        avg = (float)(totalAttempts) / wins;
        avg = std::round(avg * 100) / 100;
    }
    std::cout << std::fixed << std::setprecision(2);
    std::stringstream stream;
    stream << avg;
    temp = stream.str();
    space = 9 - temp.size();
    std::cout << std::string(space, ' ') << temp << std::endl;
    std::cout << "Win Percentage:";
    if (played == 0)
    {
        avg = 0;
    }
    else
    {
        avg = (float)(wins) / played;
        avg = std::round(avg * 100);
    }
    std::cout << std::setprecision(0);
    stream.str("");
    stream << avg;
    temp = stream.str();
    space = 10 - temp.size();
    std::cout << std::string(space, ' ') << temp << '%' << std::endl;
    std::cout << "Current Streak:";
    temp = std::to_string(currStreak);
    space = 11 - temp.size();
    std::cout << std::string(space, ' ') << temp << std::endl;
    std::cout << "Longest Streak:";
    temp = std::to_string(maxStreak);
    space = 11 - temp.size();
    std::cout << std::string(space, ' ') << temp << std::endl;
    std::cout << "\n\n--------------------------" << std::endl;
    std::cout << "WORD     ATTEMPTS      WIN" << std::endl;
    std::cout << "--------------------------" << std::endl;
    unsigned int startingInd;
    unsigned int gamesListed = 6;
    if (games.size() < gamesListed)
    {
        startingInd = 0;
    }
    else
    {
        startingInd = games.size() - gamesListed;
    }
    for (unsigned int i = startingInd; i < games.size(); i++)
    {
        std::string currGame = games.at(i);
        currGame = toUpper(currGame);
        int wordLength = currGame[0] - '0';
        int currAttempts = std::stoi(currGame.substr(wordLength + 1, 1));
        bool currWon = (bool)(std::stoi(currGame.substr(wordLength + 2, 1)));

        std::string currWord = currGame.substr(1, wordLength);
        int space = 16 - wordLength;
        std::cout << currWord << std::string(space, ' ') << currAttempts << "      " << (currWon ? "Yes" : " No") << std::endl;
    }
    std::cout << std::endl;
}

void resetStats()
{
    std::ofstream file(statsFile);
    if (file.is_open())
    {
        file << "";
    }
    showStats();
}

#endif
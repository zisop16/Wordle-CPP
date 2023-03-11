#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <iostream>
#include "../wordle_art.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include <vector>
#include <unordered_map>

enum statusCodes
{
    awaitLaunch = 0,
    awaitGame = 1,
    gameActive = 2
};

void sleep(int milli = 10)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
}

bool getToken(std::string token)
{
    std::string tokenPath = "tokens/" + token;
    bool exists = std::filesystem::exists(tokenPath);
    if (exists)
    {
        std::remove(tokenPath.c_str());
    }
    return exists;
}

std::vector<std::string> lineColors(std::unordered_map<char, std::string> keyColors, std::string line)
{
    std::vector<std::string> colors = defaultColors(line.size());
    for (unsigned int i = 0; i < line.size(); i++)
    {
        colors[i] = keyColors.at(line[i]);
    }
    return colors;
}

/*
keyColors is a map from char to string of form:
keyColors = {
    'a': white,
    'b': green,
    'c': grey
};
*/
void displayKeyboard(std::unordered_map<char, std::string> keyColors)
{
    std::string line1 = "QWERTYUIOP";
    std::string line2 = "ASDFGHJKL";
    std::string line3 = "ZXCVBNM";
    std::vector<std::string> line1Colors = lineColors(keyColors, line1);
    std::vector<std::string> line2Colors = lineColors(keyColors, line2);
    std::vector<std::string> line3Colors = lineColors(keyColors, line3);
    std::cout << clearScreen;
    fancyLine(line1, line1Colors);
    fancyLine(line2, line2Colors, 2);
    fancyLine(line3, line3Colors, 7);
}

std::unordered_map<char, std::string> resetKeyColors()
{
    std::unordered_map<char, std::string> keyColors;
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (unsigned int i = 0; i < alphabet.size(); i++)
    {
        keyColors[alphabet[i]] = white;
    }
    return keyColors;
}
static std::string colorPath = "color.txt";
std::unordered_map<char, std::string> updateColors(std::unordered_map<char, std::string> keyColors)
{
    if (!std::filesystem::exists(colorPath))
    {
        return keyColors;
    }
    /*
    Format of color file:
    agreen
    cyellow
    tgrey
    ogrey
    ryellow
    */
    std::vector<std::string> lines;
    std::ifstream colorFile(colorPath);
    if (colorFile.is_open())
    {
        while (colorFile)
        {
            std::string currLine;
            colorFile >> currLine;
            lines.push_back(currLine);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        std::string curr = lines.at(i);
        char letter = curr[0];
        std::string color = curr.substr(1, curr.size() - 1);
        // If the key is already green, it should never be changed
        // Also, if it is already yellow, it may only be changed to green
        if (keyColors[letter] == green)
        {
            continue;
        }
        else if ((keyColors[letter] == yellow) && (color != green))
        {
            continue;
        }
        keyColors[letter] = color;
    }
    colorFile.close();
    std::remove(colorPath.c_str());
    return keyColors;
}

void runKeyboard()
{
    int status = awaitLaunch;
    std::unordered_map<char, std::string> keyColors;
    bool update = true;
    while (true)
    {
        sleep();
        if (update)
        {
            switch (status)
            {
            case awaitLaunch:
            {
                std::cout << clearScreen;
                std::cout << "Please launch the Wordle Game" << std::endl;
                sleep();
                break;
            }
            case awaitGame:
            {
                std::cout << clearScreen;
                std::cout << "Waiting for Wordle Game round to start" << std::endl;
                sleep();
                break;
            }
            case gameActive:
            {
                displayKeyboard(keyColors);
                break;
            }
            }
            update = false;
        }
        if (getToken("gameLaunch"))
        {
            status = awaitGame;
            update = true;
        }
        else if (getToken("gameStart"))
        {
            status = gameActive;
            keyColors = resetKeyColors();
            update = true;
        }
        else if (getToken("updateColors"))
        {
            keyColors = updateColors(keyColors);
            update = true;
        }
        else if (getToken("gameExit"))
        {
            status = awaitLaunch;
            update = true;
        }
    }
}

#endif
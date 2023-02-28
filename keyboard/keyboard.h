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

std::vector<int> lineColors(std::unordered_map<char, int> keyColors, std::string line) {
    std::vector<int> colors = defaultColors(line.size());
    for (unsigned int i = 0; i < line.size(); i++) {
        colors[i] = keyColors.at(line[i]);
    }
    return colors;
}

/*
keyColors is a map from char to int of form:
keyColors = {
    'a': white,
    'b': green,
    'c': grey
};
*/
void displayKeyboard(std::unordered_map<char, int> keyColors)
{
    std::string line1 = "QWERTYUIOP";
    std::string line2 = "ASDFGHJKL";
    std::string line3 = "ZXCVBNM";
    std::vector<int> line1Colors = lineColors(keyColors, line1);
    std::vector<int> line2Colors = lineColors(keyColors, line2);
    std::vector<int> line3Colors = lineColors(keyColors, line3);
    clearScreen();
    fancyLine(line1, line1Colors);
    fancyLine(line2, line2Colors, 2);
    fancyLine(line3, line3Colors, 7);
}

std::unordered_map<char, int> resetColors() {
    std::unordered_map<char, int> keyColors;
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (unsigned int i = 0; i < alphabet.size(); i++) {
        keyColors[alphabet[i]] = white;
    }
    return keyColors;
}

void runKeyboard()
{
    int status = awaitLaunch;
    std::unordered_map<char, int> keyColors;
    bool update = true;
    while (true)
    {
        sleep();
        if (update) {
            switch (status)
            {
                case awaitLaunch:
                {
                    clearScreen();
                    std::cout << "Please launch the Wordle Game" << std::endl;
                    sleep();
                    break;
                }
                case awaitGame:
                {
                    clearScreen();
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
            keyColors = resetColors();
            update = true;
        }
        else if (getToken("gameExit")) {
            status = awaitLaunch;
            update = true;
        }
    }
}

#endif
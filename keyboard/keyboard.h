#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <iostream>
#include "../wordle_art.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <cstdio>
#include <filesystem>

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
    bool exists = std::filesystem::exists(token);
    if (exists)
    {
        std::remove(token.c_str());
    }
    return exists;
}

void displayKeyboard()
{
}

void runKeyboard()
{
    int status = awaitLaunch;
    while (true)
    {
        switch (status)
        {
        case awaitLaunch:
        {
            clearScreen();
            std::cout << "Please launch the Wordle Game" << std::endl;
            sleep();
        }
        case awaitGame:
        {
            clearScreen();
            std::cout << "Waiting for Wordle Game round to start" << std::endl;
            sleep();
        }
        case gameActive:
        {
            displayKeyboard();
        }
        }
        // TODO: provide an interface for keyboard to be aware
        // that the game has been closed
        if (getToken("gameLaunch"))
        {
            status = awaitGame;
        }
        else if (getToken("gameStart"))
        {
            status = gameActive;
        }
    }
}

#endif
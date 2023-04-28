#include <iostream>
#include "utils/game_manager.h"

using namespace std;

int main(int argc, char *argv[])
{
    int numArgs = argc - 1;
    std::string allowedPath;
    std::string wordsPath;
    if (numArgs >= 1) {
        wordsPath = argv[1];
    }
    else {
        wordsPath = "data/5-words.txt";
    }
    if (numArgs >= 2) {
        allowedPath = argv[2];
    } else {
        allowedPath = "data/5-allowed.txt";
    }
    runGame(wordsPath, allowedPath);
    return 0;
}
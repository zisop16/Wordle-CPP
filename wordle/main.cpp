#include <iostream>
#include "wordle_art.h"
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    vector<int> colors = defaultColors(5);
    colors[1] = green;
    string letters = "HELLO";
    fancyLine(letters, colors);
    return 0;
}
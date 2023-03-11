#include <igloo/igloo.h>
#include "keyboard.h"
#include <iostream>
#include "../wordle_art.h"
#include "../wordle/utils/game_manager.h"
#include <unordered_map>
#include <filesystem>

using namespace igloo;

Context (Tokens) {
    Spec(CreateAndGrab) {
        generateToken("hello");
        Assert::That(getToken("hello"));
        Assert::That(!getToken("hello"));
    }
    Spec(Create2AndGrab) {
        generateToken("hello");
        generateToken("hi");
        Assert::That(!getToken("hello"));
        Assert::That(getToken("hi"));
        Assert::That(!getToken("hi"));
    }
    Spec(FolderDeletedStillFunctional) {
        std::filesystem::remove_all("./tokens");
        generateToken("gamerr");
        Assert::That(!getToken("notgamer"));
        Assert::That(getToken("gamerr"));
        Assert::That(!getToken("gamerr"));
    }
};

Context(KeyboardColors) {
    Spec(Defaults) {
        std::unordered_map<char, std::string> expected = {
            {'A', white},
            {'B', white},
            {'C', white},
            {'C', white},
            {'D', white},
            {'E', white},
            {'F', white},
            {'G', white},
            {'H', white},
            {'I', white},
            {'J', white},
            {'K', white},
            {'L', white},
            {'M', white},
            {'N', white},
            {'O', white},
            {'P', white},
            {'Q', white},
            {'R', white},
            {'S', white},
            {'T', white},
            {'U', white},
            {'V', white},
            {'W', white},
            {'X', white},
            {'Y', white},
            {'Z', white},
        };
        std::unordered_map<char, std::string> actual = resetKeyColors();
        Assert::That(actual, Equals(expected));
    }
    Spec (LineColors) {
        std::string line = "QWERTYUIOP";
        std::unordered_map<char, std::string> colors = resetKeyColors();
        colors['Q'] = grey;
        colors['U'] = green;
        colors['P'] = yellow;
        std::vector<std::string> expected = {grey, white, white, white, white, white, green, white, white, yellow};
        std::vector<std::string> actual = lineColors(colors, line);
        Assert::That(actual, Equals(expected));
    }
    Spec (UpdateColors) {
        std::unordered_map<char, std::string> actual = resetKeyColors();
        std::unordered_map<char, std::string> expected = resetKeyColors();
        actual = updateColors(actual);
        Assert::That(actual, Equals(expected));
        std::string word = "POOPY";
        std::vector<std::string> colors = {green, yellow, green, grey, yellow};
        Assert::That(!std::filesystem::exists(colorPath));
        writeColors(word, colors);
        Assert::That(std::filesystem::exists(colorPath));
        actual = updateColors(actual);
        expected['P'] = green;
        expected['O'] = green;
        expected['Y'] = yellow;
        Assert::That(actual, Equals(expected));
    }
};

int main(int argc, const char* argv[]){
    return TestRunner::RunAllTests(argc, argv);
}

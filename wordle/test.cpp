#include <igloo/igloo.h>
#include "utils/game_manager.h"
#include "utils/stats_manager.h"
#include "../wordle_art.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace igloo;

Context(GuessColors){
    Spec(TwoGreenThreeGrey){
        std::string guess = "hello";
        std::string correctWord = "hails";
        std::vector<std::string> colors = guessColors(guess, correctWord);
        Assert::That(colors[0], Equals(green));
        Assert::That(colors[1], Equals(grey));
        Assert::That(colors[2], Equals(grey));
        Assert::That(colors[3], Equals(green));
        Assert::That(colors[4], Equals(grey));
    }
    Spec(ThreeGreenTwoYellow)
    {
        std::string guess = "abbaa";
        std::string correct = "ababa";
        std::vector<std::string> expected = {
            green, green, yellow, yellow, green};
        std::vector<std::string> actual = guessColors(guess, correct);
        Assert::That(actual, Equals(expected));
    }
    Spec(TwoYellowThreeGrey)
    {
        std::string guess = "abbaa";
        std::string correct = "bccbb";
        std::vector<std::string> expected = {
            grey, yellow, yellow, grey, grey};
        std::vector<std::string> actual = guessColors(guess, correct);
        Assert::That(actual, Equals(expected));
    }
    Spec(OneYellowOneGreenThreeGrey)
    {
        std::string guess = "abbaa";
        std::string correct = "cabcc";
        std::vector<std::string> expected = {
            yellow, grey, green, grey, grey};
        std::vector<std::string> actual = guessColors(guess, correct);
        Assert::That(actual, Equals(expected));
    }
};

Context(Statistics){

    Spec(ResetStats){
        addToStats("hello", 5, true);
        resetStats();
        std::ifstream stats(statsFile);
        Assert::That(stats.is_open());
        std::string text = "";
        std::string currLine;
        while (std::getline(stats, currLine))
        {
            text += currLine;
        }
        Assert::That(text, Equals(""));
    }
    Spec(AddSomeGames)
    {
        resetStats();
        addToStats("hello", 5, true);
        addToStats("speed", 6, false);
        addToStats("poops", 6, true);
        std::ifstream stats(statsFile);
        Assert::That(stats.is_open());
        std::string actual;
        Assert(getline(stats, actual));
        std::string expected = "hello51";
        Assert::That(actual, Equals(expected));
        Assert(getline(stats, actual));
        expected = "speed60";
        Assert::That(actual, Equals(expected));
        Assert(getline(stats, actual));
        expected = "poops61";
        Assert::That(actual, Equals(expected));
    }
};

int main(int argc, const char *argv[])
{
    std::cout << "Running Tests..." << std::endl;
    return TestRunner::RunAllTests(argc, argv);
}

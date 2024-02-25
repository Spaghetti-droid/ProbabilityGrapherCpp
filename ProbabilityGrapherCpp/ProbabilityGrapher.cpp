#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include "ProbabilityGrapher.h"

int main(int argc, char* argv[])
{
    // Parse input

    if (argc == 1) {
        return 0;
    }

    if (containsCmdOption(argv, argv + argc, "-h")) {
        printUsage();
        return 0;
    }

    std::map<int, int> diceMap{};
    fillDiceMap(argc, argv, diceMap);
    int argCount = argc - 1;
    if (diceMap.size() != argCount) {
        return 1;
    }

    for (auto const& [key, val] : diceMap) {
        std::cout << key << ':' << val << '\n';
    }

    // Generate dice ranges

    // Add them together

    // Count occurences in final array

    // Output value/count pairs

}

void printUsage() {
    std::cout << "ProbabilityGrapher gives the likelyhood of each possible result for a set of dice.\n";
    std::cout << "Usage:\n";
    std::cout << "ProbabilityGrapher.exe [-h] <expr>\n";
    std::cout << "Parameters:\n";
    std::cout << "\t -h: Shows this help and quits\n";
    std::cout << "\t expr: An expression representing the set of dice that is thrown. It is composed of several ndN blocks seperated by spaces.\n\t       Each ndN block represents a set of n dice with values ranging from 1 to N.";
    std::cout << "For example, the expression '5d2 3d4 1d6' represents the combination of 5 2-sided dice, 3 4-sided, and 1 6-sided.";
}

bool containsCmdOption(char** start, char** end, const std::string& opt) {
    return std::find(start, end, opt) != end;
}

void fillDiceMap(int argc, char* argv[], std::map<int, int>& diceMap) {
    for (int i{ 1 }; i < argc; i++) {
        char* numDiceC = argv[i];
        // I do not understand why strtok is apparently deprecated according to vs, but not anyone else.
        #pragma warning(suppress : 4996)
        char* maxValueC = std::strtok(numDiceC, "d");
        #pragma warning(suppress : 4996)
        maxValueC = std::strtok(nullptr, "d");
        bool error = !maxValueC;
        if (!error) {
            try {
                const int numDice{ std::stoi(numDiceC) };
                const int maxValue{ std::stoi(maxValueC) };
                auto elem = diceMap.find(maxValue);
                if (elem == diceMap.end()) {
                    diceMap.emplace(maxValue, numDice);
                }
                else {
                    diceMap.emplace(maxValue, numDice + elem->second);
                }
            }
            catch (std::invalid_argument const& ex) {
                std::cerr << "std::invalid_argument::what(): " << ex.what() << '\n';
                error = true;
            }
            catch (std::out_of_range const& ex) {
                std::cerr << "std::out_of_range::what(): " << ex.what() << '\n';
                error = true;
            }
        }
        
        if(error) {
            std::cerr << "Argument " << i << " is malformed. Ignoring it.";
        }
    }
}







// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

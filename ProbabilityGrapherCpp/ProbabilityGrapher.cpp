#include <iostream>
#include <map>
#include <vector>
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

    std::cerr << "Received:\n";
    for (auto const& [key, val] : diceMap) {
        std::cerr << key << ':' << val << '\n';
    }

    // Generate dice ranges

    std::vector<std::vector<int>> ranges{};
    for (auto const& [maxValue, nDice] : diceMap) {
        for(int i=0; i<nDice; i++){
            std::vector<int> range{};
            range.reserve(maxValue);
            for(int val=1; val<=maxValue; val++){
                range.push_back(val);
            }
            ranges.push_back(range);
        }
    }

    // Add them together

    std::vector<int>& current = ranges[0];
    std::vector<long> previousSums{};  
    std::vector<long> currentSums(std::begin(current), std::end(current));
    for(int i=1; i<ranges.size(); i++){
        current = ranges[i];
        previousSums.swap(currentSums);
        currentSums.clear();
        for(int rangeVal : current){
            for(long sumVal : previousSums){
                currentSums.push_back(rangeVal + sumVal);
            }
        }
    }

    // Count occurences in final array

    std::map<long, long> sumToOccurences{};
    for(long s : currentSums){
        auto elem = sumToOccurences.find(s);
        if(elem == sumToOccurences.end()){
            sumToOccurences.emplace(s, 1);
            std::cerr << "Added " << s << '\n';
        } else {
            std::cerr << "Second: " << elem->second << '\n';
            sumToOccurences[s] = elem->second + 1;
            std::cerr << "Incremented " << s << '\n';
        }
    }

    // Output value/count pairs

    for (auto const& [sum, occurences] : sumToOccurences) {
        std::cout << sum << ':' << occurences << '\n';
    }

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
                error = numDice <= 0 || maxValue <= 0;
                if(!error){ 
                    auto elem = diceMap.find(maxValue);
                    if (elem == diceMap.end()) {
                        diceMap.emplace(maxValue, numDice);
                    }
                    else {
                        diceMap.emplace(maxValue, numDice + elem->second);
                    }
                } else {
                    std::cerr << "Negative or 0 value detected\n";
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

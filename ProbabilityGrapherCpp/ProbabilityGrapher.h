#pragma once

// Print usage information
void printUsage();
// Check to see if array contains opt
bool containsCmdOption(char** start, char** end, const std::string& opt);
/* Fill map with Max value - number of dice pairs obtained by parsing input arguments.
    diceMap: Map of Max value to number of dice to be filled 
*/
void fillDiceMap(int argc, char* argv[], std::map<int, int>& diceMap);

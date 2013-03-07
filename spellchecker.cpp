/**
 * @file spellchecker.cpp
 * @author Madison Solarana
 * @brief The spell checker program.
 * @details This program checks the spelling of user input strings using the default unix dictionary.
 * @date Wed Mar 6, 2013
 * @version 1.3
 * @copyright Academic Free License ("AFL") v. 3.0
 * Problem Description: http://www.twitch.tv/problems/spellcheck
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <string>
#include <utility>
#include <chrono>
#include <cctype>
#include <cstdlib>
#include "node.h"
#include "trie.h"

using namespace std;

/**
 * Function that determines whether or not user input is safe to search for
 * @param input - the string the user input via stdin console
 * @return returns true if the string just contains characters, false if otherwise
 */
bool isValidInput(const string& input);

int main()
{
  Trie dictionary; //Trie in which the dictionary contents are stored in
  bool exitChoice = false; //Boolean flag to control program termination
  string reader = ""; //String that will store the contents of the dictionary file in temporarily
  unsigned int numWords = 0; //Integer that holds the number of words in the dictionary
  ifstream fileIn; //Input file stream interface
  typedef std::chrono::high_resolution_clock highResClock; //typedef for convenience in writing certain expressions
  typedef std::chrono::time_point<highResClock> time; //typedef for convenience in writing certain expressions
  typedef std::chrono::milliseconds milliseconds; //typedef for convenience in writing certain expressions
  typedef std::chrono::microseconds microseconds; //typedef for convenience in writing certain expressions
  time dictionaryStartTime, dictionaryEndTime; //Time variables that will hold the starting and stopping time of the dictionary loading operation
  
  cout << "Welcome to the Spell Checker." << endl;
  
  fileIn.open("/usr/share/dict/words");
  if(fileIn.fail())
  {
    cerr << "Unable to open /usr/share/dict/words!" << endl;
    cerr << "Aborting program execution due to catastrophic failure..." << endl;
    fileIn.close();
    exit(EXIT_FAILURE);
  }
    
  dictionaryStartTime = highResClock::now();
  while(!fileIn.eof())
  {
    fileIn >> reader;
    std::transform(reader.begin(), reader.end(), reader.begin(), ::tolower); //Some words begin with uppercase letters, but we need to be case-insensitive
    dictionary.addWord(reader);
    ++numWords;
  }
  dictionaryEndTime = highResClock::now();
  fileIn.close();
    
  auto elapsedDictionaryTime = std::chrono::duration_cast<milliseconds>(dictionaryEndTime - dictionaryStartTime).count(); //store the elapsed time of the dictionary loading operation
  cout << numWords << " word(s) loaded into " << dictionary.getNodeCount() << " node(s) in " << elapsedDictionaryTime << " millisecond(s)." << endl << endl;
    
  do
  {
    time correctionStartTime, correctionEndTime; //Time variables that will hold the starting and stopping time if the correction operation
    std::set<string> corrections;
    string userInput = "";
    cout << "Enter a word (\'-1\' to quit):" << endl;
    cout << " > ";
    cin >> userInput;
    if(userInput == "-1")
    {
      exitChoice = true;
    }
    else if(isValidInput(userInput) == true)
    {
      correctionStartTime = highResClock::now();
      corrections = dictionary.getCorrections(userInput);
      correctionEndTime = highResClock::now();
      auto elapsedCorrectionTime = std::chrono::duration_cast<microseconds>(correctionEndTime - correctionStartTime).count(); //store the elapsed time of the correction operation
      if(corrections.empty() != true)
      {
        cout << corrections.size() << " possible correction(s) found in " << elapsedCorrectionTime << " microsecond(s)." << endl;
        cout << "Suggestion(s): ";
        for(const string& s : corrections)
        {
          cout << s << " ";
        }
        cout << endl;
      }
      else
      {
        cout << "No Suggestions" << endl;
      }
    }
    else
    {
      cout << "Invalid input! Please try again with a word containing only [a-z]." << endl;
    }
  }while(!exitChoice);
  
  cout << "Terminating program execution..." << endl;
  return 0;
}

bool isValidInput(const string& input)
{
  if(input == "")
  {
    return false;
  }
  else
  {
    for(const char& c : input)
    {
      if(!isalpha(c))
      {
        return false;
      }
    }
  }
  return true;
}

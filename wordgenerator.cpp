/**
 * @file wordgenerator.cpp
 * @author Madison Solarana
 * @brief The random misspelling generator program.
 * @details This program generates random misspellings of words using the default unix dictionary.
 * @date Sun Mar 3, 2013
 * @version 1.1
 * @copyright Academic Free License ("AFL") v. 3.0
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <ctime>

using namespace std;

/**
 * Function that determines if a character is a vowel
 * @param c - the character that we are examining
 * @return returns a boolean flag that represents whether or not the character is a vowel
 */
bool isVowel(const char& c);

int main()
{
  srand(time(NULL)); //Just need a simple random number generator, the generators in <random> do more than I require
  ifstream fileIn; //Input file stream interface
  ofstream fileOut; //Output file stream interface
  string reader = ""; //String that reads in the contents of the dictionary
  vector<string> dictionary; //Vector of strings that holds the contents of the dictionary
  
  fileIn.open("/usr/share/dict/words");
  if(fileIn.fail())
  {
    cerr << "Unable to open /usr/share/dict/words!" << endl;
    cerr << "Aborting program execution due to catastrophic failure..." << endl;
    fileIn.close();
    exit(EXIT_FAILURE);
  }
  while(!fileIn.eof())
  {
    fileIn >> reader;
    dictionary.push_back(reader);
  }
  fileIn.close();
  fileOut.open("wordsgenerated.txt");
  if(fileOut.fail())
  {
    cerr << "Unable to open wordsgenerated.txt!" << endl;
    cerr << "Aborting program execution due to catastrophic failure..." << endl;
    fileIn.close();
    exit(EXIT_FAILURE);
  }
  for(int i = 0; i < 50; ++i)
  {
    string startingWord = dictionary[rand() % dictionary.size()]; //Pick a random word from the dictionary
    string misspelledWord = ""; //New word that we will create
    for(const char& c : startingWord)
    {
      int randomValue = rand() % 10; //Returns an integer in the range [0,9]
      if((randomValue < 3) && (isVowel(c) == true)) //20% chance to change vowel
      {
        switch(rand() % 5) //Returns an integer in the range [0,4]
        {
          case 0:
            misspelledWord += 'u';
            break;
          case 1:
            misspelledWord += 'o';
            break;
          case 2:
            misspelledWord += 'i';
            break;
          case 3:
            misspelledWord += 'e';
            break;
          case 4:
            misspelledWord += 'a';
            break;
        }
      }
      else if((randomValue > 3) && (randomValue < 6)) //20% chance to double a letter
      {
        misspelledWord += c;
        misspelledWord += c;
      }
      else if(randomValue > 7) //20% chance to change a character to uppercase
      {
        misspelledWord += toupper(c);
      }
      else
      {
        misspelledWord += c;
      }
    }
    cout << misspelledWord << endl;
    fileOut << misspelledWord << " -> " << startingWord << endl;
  }
  fileOut.close();
  cout << "-1" << endl; //Send termination character to the receiving spell checking program
  return 0;
}

bool isVowel(const char& c)
{
  return ((c == 'a') || (c == 'e') || (c == 'i') || (c == 'o') || (c == 'u'));
}

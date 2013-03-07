/**
 * @file trie.h
 * @author Madison Solarana
 * @brief The Trie data structure.
 * @details This class is the definition of a Trie used by the spell checker to hold the dictionary.
 * @date Thu Mar 7, 2013
 * @version 1.4
 * @copyright Academic Free License ("AFL") v. 3.0
 */
#ifndef TRIE_H
#define TRIE_H
#include <algorithm>
#include <set>
#include <string>
#include <utility>
#include <cctype>
#include "node.h"

extern const unsigned short ALPHABET; ///Constant for the number of alphabetic characters defined in node.h

class Trie
{
  public:
    /**
     * Constructor that initializes the root of the Trie and sets the initial node count
     */
    Trie()
    {
      root = new Node();
      nodeCount = 0;
    }
  
    /**
     * Move Constructor that defines move semantics (rvalue reference) so that we can detect rvalue arguments via function overloading
     * @param t - a rvalue reference to a Trie
     */
    Trie(Trie&& t) : Trie()
    {
      std::swap(*this, t);
    }
  
    //Copy Constructor
    Trie(const Trie& t)
    {
      this->nodeCount = t.nodeCount;
      *this->root = *t.root;
    }
  
  /**
   * Assignment Operator Overload that defines how we set a Trie equal to another Trie using the Copy-and-Swap Idiom
   * @param t - the Trie that we are setting the other Trie equal to
   * @return returns a pointer to the instance of the Trie calling the assignment operator
   */
    Trie& operator=(const Trie& t)
    {
      if(this != &t) //Verify that this and t are not the same object
      {
        Trie temp(t);
        std::swap(*this, temp);
      }
      return *this;
    }
  
    /**
     * Destructor that deletes the dynamic memory contained in the Trie
     */
    ~Trie()
    {
      delete root;
    }
  
    /**
     * Function that returns the number of nodes currently in the Trie
     * @return returns an unsigned integer that contains the number of nodes in the Trie
     */
    unsigned int getNodeCount() const
    {
      return nodeCount;
    }
  
    /**
     * Function that adds a word to the Trie. Duplicate insertion is allowed, but no new nodes are created
     * @param word - the word to be added into the tree
     */
    void addWord(const std::string& word)
    {
      Node *currentNode = root;
      for(const char& c : word)
      {
        if(!isalpha(c))
        {
          continue; //Ignore non-alphabetic characters
        }
        if(currentNode->getChild(c) == nullptr)
        {
          currentNode->getChild(c) = new Node();
          ++nodeCount;
        }
        currentNode = currentNode->getChild(c);
      }
      currentNode->word = word;
      currentNode->isEndpoint = true;
    }
  
    /**
     * Function that searches the Trie for a specific word
     * @param word - the word that we are searching for
     * @return returns true or false if the word is in the Trie
     */
    bool search(const std::string& word) const
    {
      Node *currentNode = root;
      for(const char& c : word)
      {
        if(!isalpha(c))
        {
          continue; //Ignore non-alphabetic characters
        }
        if(currentNode->getChild(c) == nullptr)
        {
          return false;
        }
        currentNode = currentNode->getChild(c);
      }
      return currentNode->isEndpoint;
    }
  
    /**
     * Function that returns the possible corrections for a word. If there are no corrections, the empty set is returned
     * @param word - the word that needs to be corrected
     * @return returns a set of strings that are possible corrections for the word. Returns the empty set if no corrections are found
     */
    std::set<std::string> getCorrections(const std::string& word) const
    {
      std::set<std::string> results;
      std::string temp = word;
      std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
      if(search(temp) == true)
      {
        results.insert(temp);
        return results;
      }
      fuzzySearch(temp, root, results);
      return results;
    }
  
  private:
    /**
     * Function that determines if a character is a vowel
     * @param c - the character that we are examining
     * @return returns a boolean flag that represents whether or not the character is a vowel
     */
    bool isVowel(const char& c) const
    {
      return ((c == 'a') || (c == 'e') || (c == 'i') || (c == 'o') || (c == 'u'));
    }
  
    /**
     * Function that counts the number of consecutive occurrences of a letter
     * @param word - the word that is being searched
     * @param wordSize - the size of the word being searched
     * @param letter - the letter being compared to the contents of the word
     * @return returns the number of duplicate letters that follow the input letter
     */
    unsigned short getNumberOfContinuousDuplicates(const std::string& word, const unsigned short& wordSize, const char& letter) const
    {
      unsigned short numDuplicates = 0;
      for(unsigned short i = 1; i <= (wordSize - 1); ++i)
      {
        if(word[i] == letter)
        {
          ++numDuplicates;
        }
        else
        {
          break;
        }
      }
      return numDuplicates;
    }
  
    /**
     * Function that determines possible corrections for a word based off of the known error algorithm
     * @param word - the word that we are determining corrections for
     * @param currentNode - a pointer to the node we are currently at in the Trie
     * @param results - the set of results that we store corrections in
     */
    void fuzzySearch(std::string word, Node *currentNode, std::set<std::string>& results) const
    {
      unsigned short wordSize = static_cast<unsigned short>(word.size());
      if(wordSize == 0)
      {
        if(currentNode->isEndpoint == true)
        {
          results.insert(currentNode->word);
        }
        return;
      }
      
      char c = word[0];
      if(currentNode->getChild(c) != nullptr)
      {
        if(!isVowel(c)) //Don't need branching behavior for consonants
        {
          if(wordSize > 1)
          {
            unsigned short numDuplicates = getNumberOfContinuousDuplicates(word, wordSize, c);
            if(numDuplicates > 0)
            {
              for(unsigned short i = 1; i <= numDuplicates; ++i)
              {
                fuzzySearch(word.substr(i), currentNode->getChild(c), results); //Check to see if it is a valid duplicate letter
              }
              fuzzySearch(word.substr(numDuplicates + 1), currentNode->getChild(c), results); //Skip duplicate letters
            }
            else
            {
              fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
            }
          }
          else
          {
            fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
          }
        }
        else
        {
          if(currentNode->getChild('a')!= nullptr)
          {
            if(wordSize > 1)
            {
              unsigned short numDuplicates = getNumberOfContinuousDuplicates(word, wordSize, c);
              if(numDuplicates > 0)
              {
                for(unsigned short i = 1; i <= numDuplicates; ++i)
                {
                  fuzzySearch(word.substr(i), currentNode->getChild(c), results); //Check to see if it is a valid duplicate letter
                }
                fuzzySearch(word.substr(numDuplicates + 1), currentNode->getChild(c), results); //Skip duplicate letters
              }
              else
              {
                fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
              }
            }
            else
            {
              fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
            }
          }
          if(currentNode->getChild('e') != nullptr)
          {
            c = 'e';
            if(wordSize > 1)
            {
              unsigned short numDuplicates = getNumberOfContinuousDuplicates(word, wordSize, c);
              if(numDuplicates > 0)
              {
                for(unsigned short i = 1; i <= numDuplicates; ++i)
                {
                  fuzzySearch(word.substr(i), currentNode->getChild(c), results); //Check to see if it is a valid duplicate letter
                }
                fuzzySearch(word.substr(numDuplicates + 1), currentNode->getChild(c), results); //Skip duplicate letters
              }
              else
              {
                fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
              }
            }
            else
            {
              fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
            }
          }
          if(currentNode->getChild('i') != nullptr)
          {
            c = 'i';
            if(wordSize > 1)
            {
              unsigned short numDuplicates = getNumberOfContinuousDuplicates(word, wordSize, c);
              if(numDuplicates > 0)
              {
                for(unsigned short i = 1; i <= numDuplicates; ++i)
                {
                  fuzzySearch(word.substr(i), currentNode->getChild(c), results); //Check to see if it is a valid duplicate letter
                }
                fuzzySearch(word.substr(numDuplicates + 1), currentNode->getChild(c), results); //Skip duplicate letters
              }
              else
              {
                fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
              }
            }
            else
            {
              fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
            }
          }
          if(currentNode->getChild('o') != nullptr)
          {
            c = 'o';
            if(wordSize > 1)
            {
              unsigned short numDuplicates = getNumberOfContinuousDuplicates(word, wordSize, c);
              if(numDuplicates > 0)
              {
                for(unsigned short i = 1; i <= numDuplicates; ++i)
                {
                  fuzzySearch(word.substr(i), currentNode->getChild(c), results); //Check to see if it is a valid duplicate letter
                }
                fuzzySearch(word.substr(numDuplicates + 1), currentNode->getChild(c), results); //Skip duplicate letters
              }
              else
              {
                fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
              }
            }
            else
            {
              fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
            }
          }
          if(currentNode->getChild('u') != nullptr)
          {
            c = 'u';
            if(wordSize > 1)
            {
              unsigned short numDuplicates = getNumberOfContinuousDuplicates(word, wordSize, c);
              if(numDuplicates > 0)
              {
                for(unsigned short i = 1; i <= numDuplicates; ++i)
                {
                  fuzzySearch(word.substr(i), currentNode->getChild(c), results); //Check to see if it is a valid duplicate letter
                }
                fuzzySearch(word.substr(numDuplicates + 1), currentNode->getChild(c), results); //Skip duplicate letters
              }
              else
              {
                fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
              }
            }
            else
            {
              fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
            }
          }
        }
      }
    }
  
    Node *root; ///Top Node of the Trie
    unsigned int nodeCount; ///Number of nodes (not counting the root as it has no data)
};

#endif

/**
 * @file trie.h
 * @author Madison Solarana
 * @brief The Trie data structure.
 * @details This class is the definition of a Trie used by the spell checker to hold the dictionary.
 * @date Thu Mar 7, 2013
 * @version 1.6
 * @copyright Academic Free License ("AFL") v. 3.0
 */
#ifndef TRIE_H
#define TRIE_H
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
      std::string builder = "";
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
        builder += c;
        currentNode->word = builder;
      }
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
      std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower); //Convert to lowercase to eliminate case errors
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
     * Function that determines possible corrections for a word based off of the known error algorithm (duplicate letters, substituted vowels)
     * @param word - the word that we are determining corrections for
     * @param currentNode - a pointer to the node we are currently at in the Trie
     * @param results - the set of results that we store corrections in
     */
    void fuzzySearch(const std::string& word, Node *currentNode, std::set<std::string>& results) const
    {
      unsigned short wordSize = static_cast<unsigned short>(word.size());
      if(wordSize == 0)
      {
        if(currentNode->isEndpoint == true) //Only add a word if it is flagged as a complete word
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
          fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
          if(wordSize < 2)
          {
            return;
          }
          c = word[1];
          if((c == word[0]) && (currentNode->getChild(c) != nullptr))
          {
            fuzzySearch(word.substr(2), currentNode->getChild(c), results); //Skip a duplicate letter
          }
        }
        else
        {
          fuzzySearch(word.substr(1), currentNode->getChild(c), results); //Recursively continue searching
          if(currentNode->getChild('a') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('a'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('e') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('e'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('i') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('i'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('o') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('o'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('u') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('u'), results); //Recursively continue searching with a vowel permutation
          }
          if(wordSize < 2)
          {
            return;
          }
          c = word[1];
          if((c == word[0]) && (currentNode->getChild(c) != nullptr))
          {
            fuzzySearch(word.substr(2), currentNode->getChild(c), results); //Skip a duplicate letter
          }
        }
      }
      else
      {
        if(isVowel(c) == true) //Only branch for vowels
        {
          if(currentNode->getChild('a') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('a'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('e') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('e'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('i') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('i'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('o') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('o'), results); //Recursively continue searching with a vowel permutation
          }
          if(currentNode->getChild('u') != nullptr)
          {
            fuzzySearch(word.substr(1), currentNode->getChild('u'), results); //Recursively continue searching with a vowel permutation
          }
          if(wordSize < 2)
          {
            return;
          }
          c = word[1];
          if((c == word[0]) && (currentNode->getChild(c) != nullptr))
          {
            fuzzySearch(word.substr(2), currentNode->getChild(c), results); //Skip a duplicate letter
          }
        }
      }
    }
  
    Node *root; ///Top Node of the Trie
    unsigned int nodeCount; ///Number of nodes (not counting the root as it has no data)
};

#endif

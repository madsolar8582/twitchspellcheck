/**
 * @file trie.h
 * @author Madison Solarana
 * @brief The Trie data structure.
 * @details This class is the definition of a Trie used by the spell checker to hold the dictionary.
 * @date Mon Mar 4, 2013
 * @version 1.2
 * @copyright Academic Free License ("AFL") v. 3.0
 */
#ifndef TRIE_H
#define TRIE_H
#include <algorithm>
#include <set>
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
      determineCorrections(word, results);
      return results;
    }
  
  private:
    /**
     * Function that removes duplicate letters from a string
     * @param word - the word that we will remove duplicates from
     * @return returns the string with all duplicate characters removed
     */
    std::string removeDuplicates(std::string word) const
    {
      unsigned long stringSize = word.size();
      if(stringSize < 2)
      {
        return word;
      }
      else
      {
        std::string temp = "";
        char previousLetter = word[0];
        char currentLetter;
        temp += previousLetter;
        for(unsigned long i = 1; i < stringSize; ++i)
        {
          currentLetter = word[i];
          if(currentLetter != previousLetter)
          {
            temp += currentLetter;
            previousLetter = currentLetter;
          }
        }
        return temp;
      }
    }
  
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
     * Function that replaces the vowels in a string with a specified vowel
     * @param word - the word that we will replace the vowels in
     * @param v - the specificed replacement vowel
     * @return returns the string with its vowels replaced
     */
    std::string replaceVowels(std::string word, const char& v) const
    {
      unsigned long stringSize = word.size();
      if(stringSize < 2)
      {
        return word;
      }
      else
      {
        std::string temp = "";
        for(const char& c : word)
        {
          if(isVowel(c) == true)
          {
            temp += v;
          }
          else
          {
            temp += c;
          }
        }
        return temp;
      }
    }
  
    /**
     * Function that determines possible corrections for a word based off of Damerau-Levenshtein distance
     * @param word - the word that we are determining corrections for
     * @param distance - the specified distance used for calculations
     * @param currentNode - a pointer to the node we are currently at in the Trie
     * @param results - the set of results that we store corrections in
     */
    void fuzzySearch(std::string word, unsigned short distance, Node *currentNode, std::set<std::string>& results) const
    {
      unsigned long stringSize = word.size();
      if(stringSize == 0)
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
        fuzzySearch(word.substr(1), distance, currentNode->getChild(c), results); //Recursively continue searching
      }
        
      if(distance < 1)
      {
        return;
      }
      
      for(unsigned short i = 0; i < ALPHABET; ++i)
        if(currentNode->children[i] != nullptr)
        {
          fuzzySearch(word, (distance - 1), currentNode->children[i], results); //Check for a deletion
          fuzzySearch(word.substr(1), (distance - 1), currentNode->children[i], results); //Check for a substitution
        }
      
      if(stringSize < 2)
      {
        return;
      }
      
      c = word[1];
      if(currentNode->getChild(c) != nullptr)
      {
        fuzzySearch(word.substr(2), (distance - 1), currentNode->getChild(c), results); //Check for an insertion
      }
      
      std::swap(word[0],word[1]); 
      fuzzySearch(word, (distance - 1), currentNode, results); //Check for a transposition
    }
  
    /**
     * Function that determines whether or not a word contains a specified letter
     * @param word - the word being searched
     * @param letter - the letter being searched for
     * @return returns a boolean flag that represents whether or not the specified letter was found
     */
    bool containsLetter(const std::string word, const char& letter) const
    {
      for(const char& c : word)
      {
        if(c == letter)
        {
          return true;
        }
      }
      return false;
    }
  
    /**
     * Function that removes undesired search results from the result set
     * @param word - the word input by the user with its duplicate letters removed
     * @param results - the set of possible matches
     * @return returns a set with either the same number or fewer elements of the search results with undesirable words removed
     */
    std::set<std::string> pruneCorrections(const std::string& word, const std::set<std::string>& results) const
    {
      std::set<std::string> temp = results;
      const char firstLetter = word[0];
      if(!isVowel(firstLetter))
      {
        for(const std::string& s : results)
        {
          char c = s[0];
          if(c != firstLetter)
          {
            temp.erase(s); //Remove words that start with a different consonant
          }
        }
      }
      else if(isVowel(firstLetter) == true)
      {
        for(const std::string& s : results)
        {
          char c = s[0];
          if(!isVowel(c))
          {
            temp.erase(s); //Remove words that start with a consonant since the input word begins with a vowel
          }
        }
      }
      
      for(const std::string& s : results)
      {
        if(s.size() < word.size())
        {
          temp.erase(s); //Remove words with less letters than the input word
        }
      }
        
      for(const std::string& s : results)
      {
        for(const char& c : s)
        {
          if(!containsLetter(word, c))
          {
            temp.erase(s); //Remove words with consonants not in the input word
            break;
          }
        }
      }
      return temp;
    }
  
    /**
     * Function that determines the possible corrections for a word
     * @param word - the word that we are finding corrections of
     * @param results - the set of possible corrections
     */
    void determineCorrections(const std::string& word, std::set<std::string>& results) const
    {
      std::string temp = word;
      std::string temp2;
      std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
      if(search(temp) == true)
      {
        results.insert(temp);
        return;
      }
      temp = removeDuplicates(temp);
      temp2 = temp;
      
      fuzzySearch(temp, 4, root, results);
      temp = replaceVowels(temp, 'a');
      fuzzySearch(temp, 3, root, results);
      temp = replaceVowels(temp, 'e');
      fuzzySearch(temp, 3, root, results);
      temp = replaceVowels(temp, 'i');
      fuzzySearch(temp, 3, root, results);
      temp = replaceVowels(temp, 'o');
      fuzzySearch(temp, 3, root, results);
      temp = replaceVowels(temp, 'u');
      fuzzySearch(temp, 3, root, results);
      
      results = pruneCorrections(temp2, results);
      return;
    }
  
    Node *root; ///Top Node of the Trie
    unsigned int nodeCount; ///Number of nodes (not counting the root as it has no data)
};

#endif

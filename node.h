/**
 * @file node.h
 * @author Madison Solarana
 * @brief The Node data structure.
 * @details This class is the definition of a Node used by the Trie data structure to hold words.
 * @date Sun Mar 3, 2013
 * @version 1.1
 * @copyright Academic Free License ("AFL") v. 3.0
 */
#ifndef NODE_H
#define NODE_H
#include <cctype>
#include <utility>

extern const unsigned short ALPHABET = 26; ///Constant that defines the number of alphabetic characters

struct Node
{
  /**
   * Constructor that initializes the array of child Nodes, sets the word endpoint delimiter to false, and initializes its string data to the empty string
   */
  Node()
  {
    for(unsigned short i = 0; i < ALPHABET; ++i)
    {
      children[i] = nullptr;
    }
    isEndpoint = false;
    word = "";
  }
  
  /**
   * Move Constructor that defines move semantics (rvalue reference) so that we can detect rvalue arguments via function overloading
   * @param n - a rvalue reference to a Node
   */
  Node(Node&& n) : Node()
  {
    std::swap(*this, n);
  }
  
  /**
   * Copy Constructor that defines how we copy a Node to another Node
   * @param n - the Node that we are copying
   */
  Node(const Node& n)
  {
    if(&n == nullptr)
    {
      Node();
    }
    else
    {
      this->isEndpoint = n.isEndpoint;
      this->word = n.word;
      for(unsigned short i = 0; i < ALPHABET; ++i)
      {
        if(&n.children[i] != nullptr)
        {
          *this->children[i] = *n.children[i];
        }
        else
        {
          this->children[i] = nullptr;
        }
      }
    }
  }
  
  /**
   * Assignment Operator Overload that defines how we set a Node equal to another Node using the Copy-and-Swap Idiom
   * @param n - the Node that we are setting the other Node equal to
   * @return returns a pointer to the instance of the Node calling the assignment operator
   */
  Node& operator=(const Node& n)
  {
    if(this != &n) //Verify that this and n are not the same object
    {
      Node temp(n);
      std::swap(*this, temp);
    }
    return *this;
  }
  
  /**
   * Destructor that cleans up the dynamic memory contained in the Node
   */
  ~Node()
  {
    for(unsigned short i = 0; i < ALPHABET; ++i)
    {
      if(children[i] == nullptr)
      {
        delete children[i];
      }
    }
  }
  
  /**
   * Function that returns a reference to a child Node so that we can access or mutate its contents
   * @param c - the character in the alphabet we are looking for
   * @return returns a reference to a child Node so that we can access or mutate its contents
   */
  Node*& getChild(const char& c)
  {
    return children[tolower(c) - 'a']; //Converts character to lowercase and then we use its alphabetical value to access the corresponding child Node
  }
  
  std::string word; ///Data contained in the node
  bool isEndpoint; ///Specifies if the node is an endpoint of a word
  struct Node *children[ALPHABET]; ///Nodes for each letter of the alphabet
};

#endif

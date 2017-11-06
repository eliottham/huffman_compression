#include "HCTree.h"
#include "HCNode.h"
#include <queue>
#include <stack>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;


/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>&freqs)
{  
  //initialize queue
  priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

  //traverse through the vector of ints
  for(unsigned int i = 0; i< freqs.size(); i++){
    //we ignore values of 0 in the tree 
    if(freqs[i] != 0){
      //create new nodes associated with each nonzero vector value
      leaves[i] = new HCNode(freqs[i], (char) i);
      //add nodes to the queue
      pq.push(leaves[i]);
    }
  }

  HCNode* node;
  HCNode* left;
  HCNode* right;
  for(unsigned int i = 0; i < pq.size(); i++) {
    left = pq.top();  // Take out the smallest node
    pq.pop();
    right = pq.top();  // Take out the second smallest node
    pq.pop();
    
    // Create new node with added freqs of nodes and symbol of left child
    node = new HCNode(left->count + right->count, left->symbol);
    
    left->p = node;  // Set smallest node's parent to new node
    right->p = node; // Set 2nd smallest node's parent to new node
    node->c0 = left; // Set node's c0 child as smallest node
    node->c1 = right; // Set node's c1 child as 2nd smallest node

    pq.push(node); // Push new node back into pq
  }
  root = node; // Set root of tree
}

/** Write to the given BitOutputStream
  *  the sequence of bits coding the given symbol.
  *  PRECONDITION: build() has been called, to create the coding
  *  tree, and initialize root pointer and leaves vector.
  */
void HCTree::encode(byte symbol, ofstream& out) const
{ 
  // Start from the leaf node of symbol
  HCNode* current = leaves[(int) symbol];
  stack<int> s;  

  // While the current node has a parent
  while(current->p) {
    // Check if current is the c0 child
    if(current->p->c0 == current) {
      s.push(0); // Push 0 to the stack if it is
    }
    // Check if current is the c1 child
    else {
      s.push(1); // Push 1 to the stack if it is
    }
    current = current->p; // Switch current to current's parent
  }

  int sValue;
  // While the stack has elements
  while(!s.empty()){
    sValue = s.top();  // Take the top element from the stack
    out << (char)sValue; // Write it to the outfile as a char
    s.pop();  // Remove the top element from the stack
  } 
}

/** Return symbol coded in the next sequence of bits from the stream.
  *  PRECONDITION: build() has been called, to create the coding
  *  tree, and initialize root pointer and leaves vector.
  */
int HCTree::decode(ifstream& in) const
{ 
  char c;
  HCNode* current = root;

  //check for not end of file
  while(in.get(c)){
    // Check if character is a 0 or 1
    if(c == 0){
      current = current ->c0; //traverse left
    }
    else{
      current = current ->c1; //traverse right
    }
  }
  return current->symbol;
}





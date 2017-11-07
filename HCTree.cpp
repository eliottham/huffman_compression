#include "HCTree.h"
#include "HCNode.h"
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
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
  for(unsigned int i = 0; i < freqs.size(); i++){
    //we ignore values of 0 in the tree 
    if(freqs[i] != 0){
      //create new nodes associated with each nonzero vector value
      leaves[i] = new HCNode(freqs[i], i);
      //add nodes to the queue
      pq.push(leaves[i]);
    }
  }

  HCNode* node;
  HCNode* left;
  HCNode* right;

  while(1) { // Continue until only the root of the tree remains
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
    if(pq.size() == 1) { // Break when the root is the only element left
      break;
    }
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
  HCNode* current = leaves[symbol];
  stack<byte> s;  

  // While the current node has a parent
  while(current->p) {
    // Check if current is the c0 child
    if(current->p->c0 == current) {
      s.push('0'); // Push 0 to the stack if it is
    }
    // Check if current is the c1 child
    else {
      s.push('1'); // Push 1 to the stack if it is
    }
    current = current->p; // Switch current to current's parent
  }

  byte sValue;
  // While the stack has elements
  while(!s.empty()){
    sValue = s.top();  // Take the top element from the stack
    //out.write((char*) &sValue, sizeof(sValue)); // TODO: unformatted(?)
    out << sValue; // Write ascii char to out (formatted)
    s.pop();  // Remove the top element from the stack
  } 
}

/** Return symbol coded in the next sequence of bits from the stream.
  *  PRECONDITION: build() has been called, to create the coding
  *  tree, and initialize root pointer and leaves vector.
  */
int HCTree::decode(ifstream& in) const
{ 
  int nextByte;
  byte nextChar;
  HCNode* current = root;

  while(1) {
    // Check if character is a 0 or 1
    nextChar = (unsigned char) in.get();
    if(nextChar == '0'){
      current = current->c0; //traverse left
    }
    else{
      current = current->c1; //traverse right
    }

    // Check if the current node is a leaf node and return symbol if so
    if(find(leaves.begin(), leaves.end(), current) != leaves.end()) {
      return current->symbol;
    }
  }
}

/* Destructor Helper */
void HCTree::deleteAll(HCNode* node)
{
  //delete the nodes in the tree
  if(node->c0) {
    deleteAll(node->c0);
  }
  if(node->c1) {
    deleteAll(node->c1);
  }
  
  delete node;
  node = nullptr;
}

/* Destructor */
HCTree::~HCTree()
{
  deleteAll(root);
}



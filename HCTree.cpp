/*PA2
 *Name 1: Khelsey Gozum // A12070231
 *Name 2: Eliott Ham // A13186685
 *Date: November 10, 2017
 *Overview of HCTree.cpp: To create a Huffman coding trie, implementing methods
 *  to build the tree, encode the tree, and decode the tree. 
 */

#include "BitOutputStream.h"
#include "BitInputStream.h"
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
      leaves[i]->isLeaf = true;
      
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
/*
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
    out << sValue; // Write ascii char to out (formatted)
    s.pop();  // Remove the top element from the stack
  } 
}
*/
/** Write to the given BitOutputStream
  *  the sequence of bits coding the given symbol.
  *  PRECONDITION: build() has been called, to create the coding
  *  tree, and initialize root pointer and leaves vector.
  */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
  HCNode* current = leaves[symbol]; // Start from the leaf node of symbol
  stack<unsigned char> s;  

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

  unsigned char sValue;

  // While the stack has elements
  while(!s.empty()){
    sValue = s.top();  // Take the top element from the stack
    out.writeBit(sValue); // Send value of prefix code to be written as a bit
    s.pop();  // Remove the top element from the stack
  } 
}

//Create an encoded version of the tree as the header of the compressed file
void HCTree::encodeTree(ofstream& out)
{
  HCNode* node = root;
  vector<char> encodedTree;
  queue<HCNode*> q;
  q.push(node);

  //Do BFS to get structure of tree
  while(!q.empty()) {
    node = q.front(); //Take the front node of the queue
    q.pop();
  
    if(node->isLeaf) { //If it is a leaf push a 1 before its symbol
      encodedTree.push_back('1');
      encodedTree.push_back(node->symbol);
    }
    else {
      encodedTree.push_back('0'); //Push a 0 to represent non-leaf node
    }
   
    if(node->c0) { //Push the 0 child if it exists into the queue
      q.push(node->c0);
    }

    if(node->c1) { //Push the 1 child if it exists into the queue
      q.push(node->c1);
    }
  }
  
  //Tell the decoder how many bytes represent the header
  int etsize = (int) encodedTree.size();
  out.write((char*) &etsize, sizeof(etsize));

  for(unsigned int i = 0; i < encodedTree.size(); i++) {
    out.put(encodedTree[i]); //Print the structure of the tree
  }
}

/** Return symbol coded in the next sequence of bits from the stream.
  *  PRECONDITION: build() has been called, to create the coding
  *  tree, and initialize root pointer and leaves vector.
  */
/*
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
*/

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::decode(BitInputStream& in, ofstream& out, int sigBits) const
{
  HCNode* current = root;
  int nextBit;

  //while(in.notEOF()) {
    for(unsigned int i = 0; i < sigBits; i++) {
      nextBit = in.readBit();

      if(nextBit == 0) {
        current = current->c0;
      }
      else {
        current = current->c1;
      }

      if(current->isLeaf) {
        out << current->symbol;
        current = root;
      }
    }
  //}    
}

void HCTree::decodeTree(queue<unsigned char> encodedTree) 
{
  unsigned char treeChar;
  queue<HCNode*> nlNodes; //Queue for holding non-leaf nodes
  HCNode* node;
  HCNode* c0;
  HCNode* c1;
  unsigned char symbol;

  //Build the root of the tree
  root = new HCNode(0, 0);
  encodedTree.pop(); //The first char in encodedTree represents the root
  nlNodes.push(root);

  //Loop through the entire queue to build the tree
  while(!encodedTree.empty()) {
    node = nlNodes.front(); //Set nonleaf node as current node
    nlNodes.pop();

    treeChar = encodedTree.front();
    encodedTree.pop();

    //Assign the current non-leaf node's c0 child
    if(treeChar == '0') { //0's represent non-leaf nodes
      c0 = new HCNode(0, 0); //Create a nl node as current node's c0
      c0->p = node; //Set the parent child relation
      node->c0 = c0;
      nlNodes.push(c0); //Push the nl node into the queue of nl nodes
    }
    else { //1's represent leaf nodes with the following char as its symbol
      symbol = encodedTree.front();
      encodedTree.pop();
      c0 = new HCNode(0, symbol); //Create the leaf node
      c0->p = node; //Set the parent child relation
      node->c0 = c0;
      c0->isLeaf = true; //Show it is a leaf
    }

    treeChar = encodedTree.front();
    encodedTree.pop();

    //Assign the current non-leaf node's c1 child
    if(treeChar == '0') { //0's represent non-leaf nodes
      c1 = new HCNode(0, 0); //Create a nl node as current node's c0
      c1->p = node; //Set the parent child relation
      node->c1 = c1;
      nlNodes.push(c1); //Push the nl node into the queue of nl nodes
    }
    else { //1's represent leaf nodes with the following char as its symbol
      symbol = encodedTree.front();
      encodedTree.pop();
      c1 = new HCNode(0, symbol); //Create the leaf node
      c1->p = node; //Set the parent child relation
      node->c1 = c1;
      c1->isLeaf = true; //Show it is a leaf
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



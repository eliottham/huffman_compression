/*
 * Overview of HCTree.cpp: To create a Huffman coding trie, implementing methods
 * to build the tree, encode the tree, and decode the tree. 
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
  // initialize queue
  priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

  // traverse through the vector of ints
  for(unsigned int i = 0; i < freqs.size(); i++){
    // we ignore values of 0 in the tree 
    if(freqs[i] != 0){
      // create new nodes associated with each nonzero vector value
      leaves[i] = new HCNode(freqs[i], i);
      leaves[i]->isLeaf = true;
      
      // add nodes to the queue
      pq.push(leaves[i]);
    }
  }

  HCNode* node;
  HCNode* left;
  HCNode* right;

  // continue until only the root of the tree remains
  if(pq.size() >= 3) {
    while(1) { 
      left = pq.top();  // take out the smallest node
      pq.pop();
      right = pq.top();  // take out the second smallest node
      pq.pop();
    
      // create new node with added freqs of nodes and symbol of left child
      node = new HCNode(left->count + right->count, left->symbol);
    
      left->p = node;  // set smallest node's parent to new node
      right->p = node; // set 2nd smallest node's parent to new node
      node->c0 = left; // set node's c0 child as smallest node
      node->c1 = right; // set node's c1 child as 2nd smallest node

      pq.push(node); // push new node back into pq
      if(pq.size() == 1) { // break when the root is the only element left
        break;
      }
    }
    root = node; // set root of tree
  }
  else {
    node = pq.top(); // make the root the entire tree
    pq.pop();
    root = node;
  }
}

/** Write to the given BitOutputStream
  *  the sequence of bits coding the given symbol.
  *  PRECONDITION: build() has been called, to create the coding
  *  tree, and initialize root pointer and leaves vector.
  */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
  HCNode* current = leaves[symbol]; // start from the leaf node of symbol
  stack<unsigned char> s; 
 
  if(current == root) { 
    s.push(0);
  }

  // while the current node has a parent
  while(current->p) {
    // check if current is the c0 child
    if(current->p->c0 == current) {
      s.push(0); // push 0 to the stack if it is
    }
    // check if current is the c1 child
    else {
      s.push(1); // push 1 to the stack if it is
    }
    current = current->p; // switch current to current's parent
  }

  unsigned char sValue;

  // while the stack has elements
  while(!s.empty()){
    sValue = s.top();  // take the top element from the stack
    out.writeBit(sValue); // send value of prefix code to be written as a bit
    s.pop();  // remove the top element from the stack
  } 
}

/**Create an encoded version of the tree as the header of the compressed file*/
void HCTree::encodeTree(ofstream& out)
{
  HCNode* node = root;
  vector<char> encodedTree;
  queue<HCNode*> q;
  q.push(node);

  // use BFS to get structure of tree
  while(!q.empty()) {
    node = q.front(); // take the front node of the queue
    q.pop();
  
    if(node->isLeaf) { // if it is a leaf push a 1 before its symbol
      encodedTree.push_back('1');
      encodedTree.push_back(node->symbol);
    }
    else {
      encodedTree.push_back('0'); // push a 0 to represent non-leaf node
    }
   
    if(node->c0) { // push the 0 child if it exists into the queue
      q.push(node->c0);
    }

    if(node->c1) { // push the 1 child if it exists into the queue
      q.push(node->c1);
    }
  }
  
  // tell the decoder how many bytes represent the header
  int etsize = (int) encodedTree.size();
  out.write((char*) &etsize, sizeof(etsize));

  for(unsigned int i = 0; i < encodedTree.size(); i++) {
    out.put(encodedTree[i]); // print the structure of the tree
  }
}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::decode(BitInputStream& in, ofstream& out, int sigBits) const
{
  HCNode* current = root;
  int nextBit;

  for(unsigned int i = 0; i < sigBits; i++) {
    nextBit = in.readBit();  // read the next bit from the bit buffer
    
    // traverse through the tree -- left child is c0 and right is c1
    if(nextBit == 0) {
      if(current->c0) {
        current = current->c0;
      }
    }
    else {
      if(current->c1) {
        current = current->c1;
      }
    }
 
    // check for if the node is a leaf node, if so gets the symbol to outfile
    if(current->isLeaf) {
      out << current->symbol;
      current = root; //reset current to root
    }
  }
}

/**Decode the header created by encodedTree**/
void HCTree::decodeTree(queue<unsigned char> encodedTree) 
{
  unsigned char treeChar;
  queue<HCNode*> nlNodes; // queue for holding non-leaf nodes
  HCNode* node;
  HCNode* c0;
  HCNode* c1;
  unsigned char symbol;

  // build the root of the tree
  root = new HCNode(0, 0);
  
  if(encodedTree.front() == '0') {
    encodedTree.pop(); // the first char in encodedTree represents the root
  }
  else { // the root is the entire tree
    encodedTree.pop(); // remove the 1 that represents it is a leaf
    root->symbol = encodedTree.front(); // set the symbol of the root (symbol following 1)
    encodedTree.pop(); 
    root->isLeaf = true;
  }
  
  nlNodes.push(root);

  // loop through the entire queue to build the tree
  while(!encodedTree.empty()) {
    node = nlNodes.front(); // set nonleaf node as current node
    nlNodes.pop();

    treeChar = encodedTree.front();
    encodedTree.pop();

    // assign the current non-leaf node's c0 child
    if(treeChar == '0') { // 0's represent non-leaf nodes
      c0 = new HCNode(0, 0); // create a nl node as current node's c0
      c0->p = node; // set the parent child relation
      node->c0 = c0;
      nlNodes.push(c0); // push the nl node into the queue of nl nodes
    }
    else { // 1's represent leaf nodes with the following char as its symbol
      symbol = encodedTree.front();
      encodedTree.pop();
      c0 = new HCNode(0, symbol); // create the leaf node
      c0->p = node; // set the parent child relation
      node->c0 = c0;
      c0->isLeaf = true; // show it is a leaf
    }

    treeChar = encodedTree.front();
    encodedTree.pop();

    // assign the current non-leaf node's c1 child
    if(treeChar == '0') { // 0's represent non-leaf nodes
      c1 = new HCNode(0, 0); // create a nl node as current node's c0
      c1->p = node; // set the parent child relation
      node->c1 = c1;
      nlNodes.push(c1); // push the nl node into the queue of nl nodes
    }
    else { // 1's represent leaf nodes with the following char as its symbol
      symbol = encodedTree.front();
      encodedTree.pop();
      c1 = new HCNode(0, symbol); // create the leaf node
      c1->p = node; // set the parent child relation
      node->c1 = c1;
      c1->isLeaf = true; // show it is a leaf
    }
  }
}

/* Destructor Helper */
void HCTree::deleteAll(HCNode* node)
{
  // delete the nodes in the tree
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



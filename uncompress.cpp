/*
 * Overview of uncompress.cpp: Reads the contents of the file named by its first
 * command line argument, created by compress, and reconstructs the original
 * uncompressed version written to the second command line argument.  Uses 
 * Bitwise I/O */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "HCNode.h"
#include "HCTree.h"
#include <stdio.h>
using namespace std;

int main(int argc, char* argv[]) {
  // check if contains proper amount of arguments
  if(argc != 3){
    cerr << "Expected arguments: ./uncompress infile outfile" << endl;
    return -1;
  }

  // open the input file
  ifstream infile;
  infile.open(argv[1], ios::binary);

  // check if file exists
  if(!infile.is_open()) {
    cerr << "File does not exist!" << endl;
    return -1;
  }

  /*Get the last char which shows how many of the 0's in the last byte of 
    encode are pad bits*/
  infile.seekg(-1, ios_base::end);
  int endPos = infile.tellg(); // mark where the Huffman code ends
  int numPadBits = infile.get();  // get number of padbits

  infile.seekg(0, ios_base::beg); // move stream back to the start of the file
  
  // get the first char, shows how many of the following bytes are for the header
  int headerSize;
  infile.read((char*) &headerSize, sizeof(headerSize));

  int nextByte;
  unsigned char nextChar;

  // loop through the header to get the encoded tree structure
  queue<unsigned char> encodedTree;
  for(int i = 0; i < headerSize; i++) {
    nextByte = infile.get();
    nextChar = (unsigned char) nextByte;
    encodedTree.push(nextChar);
  }

  int startPos = infile.tellg(); // mark where the Huffman code begins
  int sigBits = ((endPos - startPos) * 8) - numPadBits; // how many bits to read
  
  // build the huffman coding tree
  HCTree* hct = new HCTree();
  hct->decodeTree(encodedTree); 

  // open the output file
  ofstream outfile;
  outfile.open(argv[2]);

  BitInputStream* in = new BitInputStream(infile);
 
  // decode the tree 
  hct->decode(*in, outfile, sigBits);

  delete in;
  delete hct;
  // close the in and out files
  infile.close();
  outfile.close();
}

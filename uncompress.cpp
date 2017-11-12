#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "HCNode.h"
#include "HCTree.h"
#include <stdio.h>
using namespace std;


int main(int argc, char* argv[]){
  //Check if contains proper amount of arguments
  if(argc != 3){
    cerr << "Expected arguments: ./uncompress infile outfile" << endl;
    return -1;
  }

  //open the input file
  ifstream infile;
  infile.open(argv[1], ios::binary);
  
  int nextByte;
  unsigned char nextChar;

  //Get the first char which shows how many of the following bytes are for the header
  int headerSize;
  int numPadBits;

 // numPadBits = infile.get();
  //infile.read((char*) &headerSize, sizeof(headerSize));
  headerSize = infile.get();

  //Loop through the header to get the encoded tree structure
  queue<unsigned char> encodedTree;
  for(int i = 0; i < headerSize; i++) {
    nextByte = infile.get();
    nextChar = (unsigned char) nextByte;
    encodedTree.push(nextChar);
  }
  
  //build the huffman coding tree
  HCTree* hct = new HCTree();
  hct->decodeTree(encodedTree);


  //open the output file
  ofstream outfile;
  outfile.open(argv[2]);

  BitInputStream* in = new BitInputStream(infile);
/*
  int startPos = infile.tellg();
  infile.seekg(ios_base::end);
  int endPos = infile.tellg();
  int numBits = endPos - startPos;
  numBits = (numBits * 8) - numPadBits;
  infile.seekg(startPos);
*/
  hct->decode(*in, outfile);
  infile.close();
  outfile.close();

}

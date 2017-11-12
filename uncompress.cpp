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

  //Open the input file
  ifstream infile;
  infile.open(argv[1], ios::binary);

  //Get the last char which shows how many of the 0's in the last byte of encode are pad bits
  infile.seekg(-1, ios_base::end);
  int endPos = infile.tellg(); //Mark where the Huffman code ends
  int numPadBits = infile.get();

  infile.seekg(0, ios_base::beg); //Move stream back to the start of the file
  //Get the first char which shows how many of the following bytes are for the header
  //int headerSize = infile.get();
  int headerSize;
  infile.read((char*) &headerSize, sizeof(headerSize));

  int nextByte;
  unsigned char nextChar;
  
  //Loop through the header to get the encoded tree structure
  queue<unsigned char> encodedTree;

  for(int i = 0; i < headerSize; i++) {
    nextByte = infile.get();
    nextChar = (unsigned char) nextByte;
    encodedTree.push(nextChar);
  }

  int startPos = infile.tellg(); //Mark where the Huffman code begins
  int sigBits = ((endPos - startPos) * 8) - numPadBits; //Let decode know how many bits to read
  
  //Build the huffman coding tree
  HCTree* hct = new HCTree();
  hct->decodeTree(encodedTree); 


  //Open the output file
  ofstream outfile;
  outfile.open(argv[2]);

  BitInputStream* in = new BitInputStream(infile);

  hct->decode(*in, outfile, sigBits);
  infile.close();
  outfile.close();

}

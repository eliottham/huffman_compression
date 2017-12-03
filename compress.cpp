/*
 * Overview of compress.cpp: To compress small files using Bitwise I/O and
 * successfully encode the input file  The file reads the content of the
 * infile, constructs a huffman code for the contents of that file, and
 * encodes the contens into the outfile 
 */

#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include "HCNode.h"
#include "HCTree.h"
using namespace std;

int main(int argc, char* argv[]) {
  // check if contains proper amount of arguments
  if(argc != 3){
    cerr << "Expected arguments: ./compress infile outfile" << endl;
    return -1;
  }
  
  // open the infile to read the contents 
  ifstream infile;
  infile.open(argv[1], ios::binary);

  // check if file exists
  if(!infile.is_open()) {
    cerr << "File does not exist!" << endl;
    infile.close();
    return -1;
  }

  // check if empty file
  infile.seekg(0, ios::end);
  if(infile.tellg() == 0) {
    cerr << "Infile is empty!" << endl;
    infile.close();
    return -1;
  }
  infile.seekg(0,ios::beg); // reset file to the beginning
 
  // construct a huffman code tree for the contents of that file 
  HCTree* hct = new HCTree();

  vector<int> freqs(256, 0); // vector to hold frequencies of each byte
  byte nextChar; // variable to hold the byte read in
  int nextByte; // variable to check eof

  // read the contents of infile, count the number of occurrences of each byte file
  while((nextByte = infile.get()) != EOF) {
    nextChar = (unsigned char) nextByte;
    freqs[nextChar]++;
  }

  infile.close(); // close the infile

  hct->build(freqs); // construct the Huffman coding tree

  ofstream outfile; // open the output file for writing
  outfile.open(argv[2], ios::binary);
  
  hct->encodeTree(outfile); // encode the tree as the header for compressed file 

  BitOutputStream* out = new BitOutputStream(outfile);
  infile.open(argv[1], ios::binary);
 
  // translate each byte from input file into its code and append to outfile
  while((nextByte = infile.get()) != EOF) {
    nextChar = (unsigned char) nextByte;
    hct->encode(nextChar, *out); // write the encoded byte to the outfile
  }
  out->padLastByte();

  delete out;
  delete hct;
  // close the input and output files
  infile.close();
  outfile.close();
  
}


#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include "HCNode.h"
#include "HCTree.h"
using namespace std;

bool sortHelper(const pair<unsigned char, int> &pair1, const pair<unsigned char, int> &pair2);

int main(int argc, char* argv[]){
  //Check if contains proper amount of arguments
  if(argc != 3){
    cerr << "Expected arguments: ./compress infile outfile" << endl;
    return -1;
  }
   //TODO: empty file
  //TODO:Check for if file contains only one character repeated many times
  
  //open the infile to read the contents 
  ifstream infile;
  infile.open(argv[1], ios::binary);
/*
  //empty file check
  infile.seekg(0,ios::end);
  if(infile.tellg() == 0) {
    cerr << "Infile is empty!" << endl;
    return -1;
  }

  //check if file exists
  if(!infile.good()) {
    cerr << "File does not exist!" << endl;
    return -1;
  }
*/ 
  //construct a huffman code tree for the contents of that file 
  HCTree* hct = new HCTree();

  vector<int> freqs(256, 0); //vector to hold frequencies of each byte
  byte nextChar; //variable to hold the byte read in
  int nextByte; //variable to check eof

  //read the contents of infile, count the number of occurrences of each byte file
  while((nextByte = infile.get()) != EOF) {
    nextChar = (unsigned char) nextByte;
    freqs[nextChar]++;
  }

  //close the file
  infile.close();

  //construct the Huffman coding tree
  hct->build(freqs);

/* Inefficient Header  
  //write the file header to the output file
  for(unsigned int i = 0; i < freqs.size(); i++){
    outfile << freqs[i] << endl;/
    //outfile.write((char*) &freqs[i], sizeof(freqs[i]));
  }
*/

    //open the output file for writing
  ofstream outfile;
  outfile.open(argv[2], ios::binary);
  hct->encodeTree(outfile);

  BitOutputStream* out = new BitOutputStream(outfile);
  //translate each byte from input file into its code and append to outfile
  infile.open(argv[1], ios::binary);
 
  while((nextByte = infile.get()) != EOF) {
    nextChar = (unsigned char) nextByte;
    hct->encode(nextChar, *out); //write the encoded byte to the outfile
  }
  //out->padLastByte();

  //close the input and output files
  infile.close();
  outfile.close();

}

/*Helper function to sort out the frequencies of the chars from lowest
  to highest frequency*/
bool sortHelper(const pair<unsigned char, int> &pair1, const pair<unsigned char, int> &pair2)
{
  return pair1.second < pair2.second;
}

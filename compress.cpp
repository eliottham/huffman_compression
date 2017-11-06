#include <fstream>
#include <iostream>
#include "HCNode.h"
#include "HCTree.h"
using namespace std;


int main(int argc, char* argv[]){
  //Check if contains proper amount of arguments
  if(argc != 3){
    cerr << "Expected arguments: ./compress infile outfile" << endl;
    return -1;
  }

  //TODO:Check for if file contains only one character repeated many times
  
  int nextByte;
 
  //vector to hold the frequencies of each byte
  vector<int> freqs(256, 0);

  //open the contents of the file infile and outfile 
  ifstream infile;
  infile.open(argv[1], ios::binary);

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
 
  ofstream outfile;
  outfile.open(argv[2], ios::binary);

  //construct a huffman code for the contents of that file 
  HCTree* hct = new HCTree();

  //read the contents of infile, count the number of occurrences of each byte file
  while(infile.good()) {
    nextByte = (int) infile.get();
    freqs[nextByte]++;
  }

  //close the file
  infile.close();

  //construct the Huffman coding tree
  hct->build(freqs);

  //write the file header to the output file
  for(unsigned int i = 0; i < freqs.size(); i++){
    outfile << freqs[i] << endl;
  }

  //translate each byte from input file into its code and append to outfile
  infile.open(argv[1], ios::binary);
  
  while(infile.good()) {
    hct->encode(infile.get(), outfile);
  }

  //close the input and output files
  infile.close();
  outfile.close();


}

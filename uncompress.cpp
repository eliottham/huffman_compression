#include <fstream>
#include <iostream>
#include <string>
#include "HCNode.h"
#include "HCTree.h"
using namespace std;


int main(int argc, char* argv[]){
  //Check if contains proper amount of arguments
  if(argc != 3){
    cerr << "Expected arguments: ./uncompress infile outfile" << endl;
    return -1;
  }

  //open the input file
  ifstream infile;
  infile.open(argv[1]);

  //read the file header, and reconstruct the huffman coding tree
  vector<int> freqs(256,0); //frequencies of each byte
  string line; //string used to obtain each byte per line
 
  for(unsigned int i = 0; i < freqs.size(); i++){
    getline(infile, line);
    freqs[i] = stoi(line); //get the freq of the corresponding byte
  }
  
  //build the huffman coding tree
  HCTree* hct = new HCTree();
  hct->build(freqs);

  //open the output file
  ofstream outfile;
  outfile.open(argv[2]);

  //decode the bits from the input file and write to the output file
  while(infile.peek() != EOF) {
    outfile << (unsigned char) hct->decode(infile);
  }

  infile.close();
  outfile.close();
}

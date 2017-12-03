/*
 * Overview of BitInputStream.cpp: Implentation of bitwise file I/O, using 
 * iostream classes to read one bit at a time and return the appropriate
 * int.
 */

#include <iostream>
#include "BitInputStream.h"
#include "HCTree.h"
using namespace std;

/**Constructor**/
BitInputStream::BitInputStream(istream & is) : in(is), buf(0), nbits(8){}

/** Fill the buffer from the input */
void BitInputStream::fill() {
  buf = in.get();
  nbits = 0;
}

/** Read the next bit from the bit buffer.
 * Fill the buffer from the input stream first if needed
 * @return 1 if the bit read is 1
 * @return 0 if the bit read is 0
 */
int BitInputStream::readBit() {
  // if all the bits in the buffer are read, fill the buffer first
  if(nbits == 8) {
        fill();
  }

  // bit at the location in the bit buffer, and return the appropriate int
  int bit;
  bit = (buf >> nbits ) & 1;
  
  nbits++; // increment the index
  
  return bit;
}

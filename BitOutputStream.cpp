/*PA2
 *Name 1: Khelsey Gozum // A12070231
 *Name 2: Eliott Ham // A13186685
 *Date: November 10, 2017
 *Overview of BitOutputStream.cpp: Implementation of bitwise I/O, using 
 *  iostream classes to write one bit at a time
 */

#include <iostream>
#include <fstream>
#include "BitOutputStream.h"
using namespace std;

//Constructor
BitOutputStream::BitOutputStream(ostream & os) : out(os), buf(0), nbits(0){}

/** Send the buffer to the output, and clear it*/
void BitOutputStream::flush() {
  out.put(buf);
  //out.write((char*) &buf, sizeof(buf));
  out.flush();
  buf = nbits = 0;

}
/** Write the least significant bit of the argument to
     * the bit buffer, and increment the bit buffer index.
     * But flush the buffer first, if it is full. */
void BitOutputStream::writeBit(unsigned char i) {
  //Write the least significant bit of i into the buffer at the current index
  buf = (buf & ~(1 << nbits)) | (i << nbits);
  nbits++;

    //Check if the buffer is full, flush if so
  if(nbits == 8){
    flush();
  }
}

void BitOutputStream::padLastByte() {
  
  int padbits = 8 - nbits;
  
  if(nbits != 0) {
    for(int i = 0; i < padbits; i++) {
      writeBit(0);
    }
    out.put((unsigned char) padbits);
  }
  else {
    out.put((unsigned char) 0);
  }

}
  
      
    




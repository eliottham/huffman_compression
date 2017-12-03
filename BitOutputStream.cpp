/*
 * Overview of BitOutputStream.cpp: Implementation of bitwise I/O, using 
 * iostream classes to write one bit at a time to a eight bit buffer
 * and flushes when the buffer is full.
 */

#include <iostream>
#include <fstream>
#include "BitOutputStream.h"
using namespace std;

/**Constructor**/
BitOutputStream::BitOutputStream(ostream & os) : out(os), buf(0), nbits(0){}

/** Send the buffer to the output, and clear it**/
void BitOutputStream::flush() {
  out.put(buf);
  out.flush();
  buf = nbits = 0;

}

/** Write the least significant bit of the argument to the bit buffer, and 
 * increment the bit buffer index. Flush the buffer first, if it is full. */
void BitOutputStream::writeBit(unsigned char i) {
  // write the least significant bit of i into the buffer at the current index
  buf = (buf & ~(1 << nbits)) | (i << nbits);
  
  nbits++; // increment the bit buffer index

  // check if the buffer is full, flush if so
  if(nbits == 8){
    flush();
  }
}

/**Helper function to pad the remaining bits in the buffer if its not fully
 *   eight bits so it can be flushed out */
void BitOutputStream::padLastByte() {
  int padbits = 8 - nbits; //get the number of pad bits 
  
  // pad the amount of bits into the buffer, if 0 indicates just been flushed
  if(nbits != 0) {
    for(int i = 0; i < padbits; i++) {
      writeBit(0);
    }
    out.put((unsigned char) padbits); // add padbits to the stream
  }
  else {
    out.put((unsigned char) 0); // add to the stream
  }
}
  
      
    




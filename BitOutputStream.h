/*
 * Overview of BitOutputStream.h: Implementation of the method declarations for 
 * bitwise I/O, using iostream classes to write one bit at a time to a eight 
 * bit buffer and flushes when the buffer is full.
 */

#ifndef BITOUTPUTSTREAM_H
#define BITOUTPUTSTREAM_H
#include <iostream>
using namespace std;

class BitOutputStream {
private:
  char buf; // one byte buffer of bits
  int nbits; // how many bits have been written to buf
  ostream & out; // reference to the output stream to use

public:
  /** Initialize a BitoutputStream that will us the given ostream for output*/
  BitOutputStream(ostream & os);
  
  /**send the buffer to the output and clear it */
  void flush();


  /** Write the least significant bit of the argument to the bit buffer, and 
   *   increment the bit buffer index. Flush the buffer first, if it is full.*/
  void writeBit(unsigned char i);

  /**Helper function to pad the remaining bits in the buffer if its not fully
   *   eight bits so it can be flushed out */
  void padLastByte();

};
#endif

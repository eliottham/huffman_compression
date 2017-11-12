/*PA2
 *Name 1: Khelsey Gozum // A12070231
 *Name 2: Eliott Ham // A13186685
 *Date: November 10, 2017
 *Overview of BitInputStream.cpp: Implentation of bitwise file I/O, using 
 *  iostream classes to read one bit at a time.
 */




#include <iostream>
#include "BitInputStream.h"
#include "HCTree.h"
using namespace std;

/**Constructor*/
BitInputStream::BitInputStream(istream & is) : in(is), buf(0), nbits(8){}

/** Fill the buffer from the input */
void BitInputStream::fill() {
    buf = in.get();
    //cout << "buf = " << buf << endl;
    nbits = 0;
}

/** Read the next bit from the bit buffer.
 * Fill the buffer from the input stream first if needed
 * @return 1 if the bit read is 1
 * @return 0 if the bit read is 0
 */
int BitInputStream::readBit() {
    //If all the bits in the buffer are read, fill the buffer first
    if(nbits == 8){
        fill();
    }

    //Get the bit at the appropriate location in the bit
    //buffer, and return the appropriate int
    int bit;
    bit = (buf >> nbits )& 1;
    //cout <<  "bit = " << bit <<endl; 
    //increment the index
    nbits++;
    //cout << "nbits = " << nbits << endl;
    return bit;

}

bool BitInputStream::notEOF() {
  return in.peek() != EOF;
}

#include <iostream>
#include <fstream>
#include "BitOutputStream.h"

using namespace std;

//Constructor
BitOutputStream(ostream & os) : out(os), buf(0), nbits(0){}

/** Send the buffer to the output, and clear it*/
void BitOutputStream::flush() {
    out.put(buf);
    out.flush();
    buf = nbits = 0;

}
/** Write the least significant bit of the argument to
     * the bit buffer, and increment the bit buffer index.
     * But flush the buffer first, if it is full. */
void BitOutputStream::writeBit(int i) {
    //Is the bit buffer full? Then flush it
    if(nbits == 8){
        flush();
    }

    //Write the least significant bit of i into the buffer at the current index
    buf = buf << 1 | (i&1);

    //increment index
    nbits++;


}


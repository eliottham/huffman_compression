#include <iostream>
#include "BitInputStream.h"
#include "HCTree.h"
using namespace std;

/**Constructor*/
BitInputStream(istream & is) : in(is){
        buf = 0; //clear buffer
        nbits = 0; //initialize bit index
}

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
    //If all the bits in the buffer are read, fill the buffer first
    if(nbits == 8){
        fill();
    }

    //Get the bit at the appropriate location in the bit
    //buffer, and return the appropriate int
    int bit;
    bit = buf & 1;

    //TODO: idk here
    buf = (buf >> nbits);

    //increment the index
    nbits++;

    return bit;

}

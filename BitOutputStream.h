#include <iostream>
using namespace std;

class BitOutputStream{
private:
    char buf; //one byte buffer of bits
    int nbits; //how many bits have been written to buf
    ostream & out; //reference to the output stream to use

public:
    /** Initialize a BitoutputStream that will us the given ostream for output*/
    BitOutputStream(ostream & os);
    /**send the buffer to the output and clear it */
    void flush();


    /** Write the least significant bit of the argument to
     * the bit buffer, and increment the bit buffer index.
     * But flush the buffer first, if it is full. */

    void writeBit(int i);

};

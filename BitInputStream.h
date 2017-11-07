#include <iostream>
using namespace std;

class BitInputStream{
private:
    char buf; //one byte bufer of bits
    int nbits; //how many bits have been read from buf
    istream & in;  //the input stream to use

public:
    /** Initialize a BitInputStream that will use the give istream for input*/
    BitInputStream(istream & is);

    /**Fill the buffer from the input*/
    void fill();

    /** Read the next bit from the bit buffer.
     * Fill the buffer from the input stream first if needed
     * @return 1 if the bit read is 1
     * @return 0 if the bit read is 0
     */
    int readBit();
};
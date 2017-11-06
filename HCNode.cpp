#include "HCNode.h"
using namespace std;

// Overload the less-than operator for HCNode
bool HCNode::operator<(const HCNode& other){
  if(this.count == other.count) {
    return this.symbol < other.symbol;
  }
  else {
    return this.count < other.symbol;
  }
}

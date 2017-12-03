/*
 * Overview of HCNode.cpp: Implementation of the less than operator of HCNode
 */ 

#include "HCNode.h"
using namespace std;

// overload the less-than operator for HCNode
bool HCNode::operator<(const HCNode& other){
  if(this->count != other.count) {
    return this->count > other.count;
  }
  return this->symbol < other.symbol;
}

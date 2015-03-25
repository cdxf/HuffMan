#pragma once
#include "Node.h"
class InternalNode : public Node{
unsigned char symbol(){
	throw "InternalNode has no symbol";
}
void symbol(unsigned char symbol){
	throw "InternalNode has no symbol";
}
};
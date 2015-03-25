#pragma once
#include "Node.h"
class LeafNode : public Node{
public:
	LeafNode(unsigned char c) : Node(){ symbol(c); }
	unsigned char symbol(){
		return this->Symbol;
	}
	void symbol(unsigned char symbol){
		this->Symbol = symbol;
	}
};
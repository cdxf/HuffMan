#pragma once
#include <iostream>
class Node
{
protected:
	unsigned char Symbol;
	int Freq = 0;
	Node* pLeft = NULL;
	Node* pRight = NULL;
public:
	Node(){
		this->Freq = 0;
	}
	Node(int freq){
		this->Freq = freq;
	}
	void incFreq(){
		Freq++;
	}
	int freq(){
		return this->Freq;
	}
	void freq(int freq){
		this->Freq = freq;
	}
	Node* left(){
		return pLeft;
	}
	Node* left(Node* node){
		this->pLeft = node;
		return pLeft;
	}
	Node* right(){
		return pRight;
	}
	Node* right(Node* node){
		this->pRight = node;
		return pRight;
	}
	virtual unsigned char symbol() = 0;
	virtual void symbol(unsigned char symbol) = 0;
	static bool cmp(Node* a, Node* b){
		if (a->symbol() < b->symbol()) return 1;
		return 0;
	}
};


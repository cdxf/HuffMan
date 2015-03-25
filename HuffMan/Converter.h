#pragma once
#include <istream>
#include "BitQueue.h"
using namespace std;
class Converter{
protected:
	BitQueue* BitStream;
public:
	Converter(){
		BitStream = new BitQueue;
	}
	~Converter(){
		delete BitStream;
	}
};
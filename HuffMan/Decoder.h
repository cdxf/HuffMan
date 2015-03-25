#pragma once
#include <iostream>
#include <sstream>
#include <ctime>
#include "Converter.h"
#include "CodeTree.h"
using namespace std;
class Decoder : public Converter{
	CodeTree CodeBook;
	int OddBits = 0;
	istream& InputStream;
	ostream& OutputStream;
	unsigned int last = 0;
public:
	Decoder(istream& InputStream, ostream& OutputStream, CodeTree& CodeBook, int OddBits) : Converter(), CodeBook(CodeBook), OddBits(OddBits), InputStream(InputStream), OutputStream(OutputStream){
		unsigned int oldPos = (unsigned int)InputStream.tellg();
		InputStream.seekg(0, ios::end);
		this->last = (unsigned int)InputStream.tellg();
		InputStream.seekg(oldPos);
	}
	void convert(){
		inputStreamToBits();
		bitsToOutputStream();
	}
	void bitsToOutputStream(){
		bool bit = 0;
		unsigned int bytes = 0;
		while (!BitStream->empty()){
			int encodedSymbol = CodeBook.getSymbol(*BitStream);
			if (encodedSymbol != -1){
				OutputStream << (unsigned char)encodedSymbol;
			}	
		}
	}
	void inputStreamToBits(){
		unsigned char c = 0;
		unsigned int currentpos = static_cast<unsigned int>(InputStream.tellg());
		while (1){
			c = InputStream.get();
			currentpos++;
			if (currentpos == last){
				c = c / static_cast<int>(pow(2, OddBits));
				char buffer[9];
				_itoa_s(c, buffer, 2);
				string BinaryString = string(8 - OddBits - strlen(buffer), '0') + string(buffer);
				(*BitStream) << BinaryString;
				break;
			}
			if (InputStream.eof()) break;
			(*BitStream) << c;
		}
	}
};
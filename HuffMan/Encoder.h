#pragma once
#include <iostream>
#include <sstream>
#include "CodeTree.h"
#include "Converter.h"
using namespace std;
class Encoder : public Converter{
	CodeTree& CodeBook;
	int OddBits = 0;
	vector<unsigned int> convertedBytes;
	vector<ifstream*> InputFiles;
	ofstream& OutputFile;
	unsigned int BitStreamOldSize = 0;
public:
	Encoder(CodeTree& CodeBook, ofstream& OutputFile) :CodeBook(CodeBook), OutputFile(OutputFile){}
	vector<unsigned int> convert(){
		vector<unsigned int> encodedBytes;
		for (unsigned int i = 0; i < InputFiles.size(); i++){
			int bytes = inputStreamToBits(*InputFiles[i]);
			encodedBytes.push_back(bytes);
		}
		bitsToOutputStream(OutputFile);
		return encodedBytes;
	}
	void addInputStream(ifstream* InputFile){
		InputFiles.push_back(InputFile);
	}
	void bitsToOutputStream(ostream& OutputStream){
		unsigned char c = 0;
		while (!BitStream->empty()){		
			(*BitStream) >> c;
			OutputStream << c;
		}
	}
	unsigned int inputStreamToBits(istream& InputStream){
		unsigned char c = 0;
		if (!InputStream.good()) return 0;
		while (1){
			c = InputStream.get();
			if (InputStream.eof()) break;
			(*BitStream) << CodeBook.getCode(c);
		}
		OddBits = (8 - BitStream->bitsize() % 8) % 8;
		unsigned int encodedBytes = BitStream->size() - BitStreamOldSize;
		BitStreamOldSize = BitStream->size();
		return encodedBytes;
	}
	int oddBits(){ return this->OddBits; }
};
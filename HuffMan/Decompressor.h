#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include "PathUtil.h"
#include "Encoder.h"
#include "Decoder.h"
#include "CanonicalCode.h"
#include "CodeTree.h"
using namespace std;
class Decompressor{
	vector<string> OutputFileNameList;
	vector<int> OutputFileSizeList;
	vector<ofstream*> OutputFileList;
	string InputFileName;
	unsigned int InputFileSize;
	ifstream* InputFile;
	string DecompressDir;
	CodeTree CodeBook;
	void openInputFile(){
		InputFile = new ifstream(InputFileName, ios::binary);
		InputFile->seekg(0, ios::end);
		InputFileSize = (unsigned int)InputFile->tellg();
		InputFile->seekg(0, ios::beg);
		char* HUF = new char[4];
		InputFile->read(HUF, 4);
		if (string(HUF) != "HUF"){
			cerr << "Wrong File Format";
		}
		else{
			delete[] HUF;
		}
		int FileCount = 0;
		InputFile->read(reinterpret_cast<char *>(&FileCount),sizeof(int));
		OutputFileNameList.resize(FileCount);
		OutputFileSizeList.resize(FileCount);
		OutputFileList.resize(FileCount);
	}
	void ReadOutputFileInfo(){
		for (unsigned int i = 0; i < OutputFileList.size(); i++){
			InputFile->read(reinterpret_cast<char*>(&OutputFileSizeList[i]), sizeof(int));
			getline(*InputFile, OutputFileNameList[i], char(0));
		}
		InputFile->ignore();
	}
	void buildCodeBook(){
		int codeBookSize = InputFile->get();
		vector<int> codeLengths;
		codeLengths.resize(256);
		if (codeBookSize != 0){
			for (int i = 0; i < codeBookSize; i++){
				unsigned char c = InputFile->get();
				int len = InputFile->get();
				codeLengths[c] = len;
			}
		}
		else{
		for (int i = 0; i < 256; i++){
			int len = InputFile->get();
			codeLengths.push_back(len);
		}
		}
		CanonicalCode canonicalcode(codeLengths);
		CodeBook = canonicalcode.buildCodeTree();
	}
	void decompress(){
		int OddBits = 0;
		InputFile->read(reinterpret_cast<char *>(&OddBits), sizeof(OddBits));
		stringstream decompressedBytes;
		Decoder decoder(*InputFile, decompressedBytes, CodeBook, OddBits);
		decoder.convert();
		for (unsigned int i = 0; i < OutputFileList.size(); i++){
			OutputFileList[i] = new ofstream(DecompressDir + DIR_SEPARATOR +  OutputFileNameList[i], ios::binary);
			char* bytesBuffer = new char[OutputFileSizeList[i]];
			decompressedBytes.read(bytesBuffer, OutputFileSizeList[i]);
			OutputFileList[i]->write(bytesBuffer, OutputFileSizeList[i]);
			OutputFileList[i]->close();
			delete[] bytesBuffer;
		}
	}
	void CreateCompressDirectory(){
		DecompressDir = PathUtil::extractDirPath(InputFileName) + "_decompressed";
		if (CreateDirectory(DecompressDir.c_str(), NULL) == ERROR_PATH_NOT_FOUND){
			cerr << "Can not create directory";
			cin.get();
			exit(0);
		}
	}
public:
	Decompressor(string InputFileName) :InputFileName(InputFileName){
		openInputFile();
		buildCodeBook();
		ReadOutputFileInfo();
		CreateCompressDirectory();
		decompress();
		//Retrive InputFile Size
	}
	void printDecompressInfo(){
		cout << "Compressed Filename: " << PathUtil::extractFileName(InputFileName) << endl;
		cout << "Compressed Filesize: " << InputFileSize << " bytes" <<endl;
		unsigned int totalFileSize = 0;
		for (unsigned int i = 0; i < OutputFileList.size(); i++){
			cout << "Decompressed Filename " << i + 1 <<" : "<< OutputFileNameList[i] << endl;
			cout << "Decompressed Filesize " << i + 1 <<" : "<< OutputFileSizeList[i] << " bytes" << endl;
			totalFileSize += OutputFileSizeList[i];
		}
		cout << "Total Decompressed Filesize: " << totalFileSize << " bytes" << endl;
		cout << "Data compression ratio: " << (double)InputFileSize*100 / totalFileSize << "%" << endl;
	}
	~Decompressor(){
		for (unsigned int i = 0; i < OutputFileList.size(); i++){
			OutputFileList[i]->close();
			delete OutputFileList[i];
		}
		InputFile->close();
		delete InputFile;
	}
};
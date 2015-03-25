#include <fstream>
#include <string>
#include <vector>
#include "Encoder.h"
#include "CanonicalCode.h"
#include "CodeTree.h"
#include "PathUtil.h"
using namespace std;
class Compressor{
	vector<string> InputFileNameList;
	vector<int> InputFileSizeList;
	vector<ifstream*> InputFileList;
	string OutputFileName;
	ofstream OutputFile;
	vector<unsigned int> bytesConverted;
	CodeTree CodeBook;
	void openInputFile(){
		for (unsigned int i = 0; i < InputFileNameList.size(); i++){
			InputFileList.push_back(new ifstream(InputFileNameList[i],ios::binary));
		}
		InputFileSizeList.resize(InputFileList.size());
	}
	void readFileSize(){
		for (unsigned int i = 0; i < InputFileList.size(); i++){
			InputFileList[i]->seekg(0, ios::end);
			InputFileSizeList[i] = (int) InputFileList[i]->tellg();
			InputFileList[i]->clear();
			InputFileList[i]->seekg(0, ios::beg);
		}
		
		
	}
	void buildCodeBook(){
		CodeTree uCodeBook;
		for (unsigned int i = 0; i < InputFileList.size(); i++){
			uCodeBook.put(*InputFileList[i]);
			InputFileList[i]->clear();
			InputFileList[i]->seekg(0, ios::beg);
		}
		uCodeBook.build();
		CanonicalCode canonicalcode(uCodeBook);
		CodeBook = canonicalcode.buildCodeTree();
	}
	void compress(){
		OutputFile.open(OutputFileName, ios::binary);
		/*File signature*/
		OutputFile.write("HUF",3);
		OutputFile.put(char(0));
		int FileCount = InputFileList.size();
		/*File count*/
		OutputFile.write(reinterpret_cast<const char*>(&FileCount), sizeof(int));
		/* CodeBook */
		vector<int> codeLengths = CodeBook.getCodeLengths();
		int n = CodeBook.countSymbol();
		/* Type 1 codebook */
		if (n < 128){
			OutputFile.put(char(n));
			for (int i = 0; i < 256; i++){
				if (codeLengths[i] != 0){
				OutputFile.put(char(i));
				OutputFile.put(char(codeLengths[i]));
				}
			}

		}
		/* Type 2 codebook */
		else{
			OutputFile.put(char(0));
			for (int i = 0; i < 256; i++){
				OutputFile.put(char(codeLengths[i]));
			}
		}
		/*file info*/
		for (unsigned int i = 0; i < InputFileList.size(); i++){
			OutputFile.write(reinterpret_cast<const char *>(&InputFileSizeList[i]), sizeof(int));
			InputFileNameList[i] = PathUtil::extractFileName(InputFileNameList[i]);
			OutputFile << InputFileNameList[i];
			OutputFile.put(char(0));
		}
		OutputFile.put(char(0));
		unsigned int OddBitPos = static_cast<int>(OutputFile.tellp());
		OutputFile.write("",sizeof(int));
		vector<unsigned int> encodedbytes;
		encodedbytes.resize(InputFileList.size());
		Encoder encoder(CodeBook, OutputFile);
		for (unsigned int i = 0; i < InputFileList.size(); i++){
			//InputFileList[i]->clear();
			//InputFileList[i]->seekg(0, ios::beg);
			//InputStream << std::noskipws <<InputFileList[i]->rdbuf();
			encoder.addInputStream(InputFileList[i]);
		}
		bytesConverted = encoder.convert();
		
		//Encoder encoder(InputStream, OutputFile, CodeBook);
		//encoder.convert();
		OutputFile.seekp(OddBitPos);
		int oddBits = encoder.oddBits();
		OutputFile.write(reinterpret_cast<const char *>(&(oddBits)), sizeof(int));
	}
	unsigned int readOutputFileSize(){
		unsigned int oldPos = (unsigned int)OutputFile.tellp();
		OutputFile.seekp(0, ios::end);
		unsigned int size = (unsigned int)OutputFile.tellp();
		OutputFile.seekp(oldPos);
		return size;
	}
public:
	Compressor(vector<string> InputFileNameList, string OutputFileName) :InputFileNameList(InputFileNameList), OutputFileName(OutputFileName){
		openInputFile();
		readFileSize();
		buildCodeBook();
		compress();
		//Retrive InputFile Size
	}
	void printCompressInfo(){
		unsigned int totalBytes = 0;
		unsigned int totalbytesConverted = 0;
		for (unsigned int i = 0; i < InputFileList.size(); i++){
		cout << "Original Filename: " << InputFileNameList[i] << endl;
		cout << "Original Filesize: " << InputFileSizeList[i] << " bytes" << endl;
		totalBytes += InputFileSizeList[i];
		cout << "Compressed Size:" << bytesConverted[i] << " bytes" << endl;
		totalbytesConverted += bytesConverted[i];
		}
		cout << endl;
		cout << "Total Original Filesize: " << totalBytes << " bytes" <<endl;
		cout << "Total Compressed Filesize: " << totalbytesConverted << " bytes" << endl;
		cout << endl;
			cout << "Compressed Filename :" << OutputFileName<< endl;
			cout << "Compressed Filesize " << readOutputFileSize() << " bytes" << endl;
			cout << "HUF Header filesize:" << readOutputFileSize() - totalbytesConverted << " bytes" << endl;
		cout << endl;
		double compressionRatio = (double)readOutputFileSize() / totalBytes;
		cout << "Data compression ratio: " << compressionRatio*100 << "%" << endl;
		cout << "Space saved: " << (1 - compressionRatio)*100 <<"%" <<endl;
	}
	~Compressor(){
		for (unsigned int i = 0; i < InputFileList.size(); i++){
			InputFileList[i]->close();
		}
		OutputFile.close();
	}
	

};
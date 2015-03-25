#include "Compressor.h"
#include "Decompressor.h"
#include "PathUtil.h"
using namespace std;
void main(int argc, char* argv[]){
	vector<string> filename;
	if (argc == 1){
		cout << "\n\tHuffman Compressor/Decompressor" << endl;
		cout << "\tWritten by: Huynh Duy Nhat Quang - HCMUS" <<endl;
		cout << "\tUsage: " << endl;
		cout << "\tDrag files to this .exe file to compress" << endl;
		cout << "\tDrag '.huf' file to this .exe file to decompress" << endl;
		cout << "\tCmd:" << endl;
		cout << "\tEncode:" << endl;
		cout << "\tHuffman -e <outputfile> <inputfile1> … <inputfile> " << endl;
		cout << "\tDecode:" << endl;
		cout << "\tHuffman -d <inputfile>" << endl;
	}
	else if (argc == 2 && PathUtil::extractFileExt(string(argv[1])) == "huf")
	{
		cout << "\n\tDecompressing, it may take some long time for big size file!" << endl;
		string CompressedFileName = string(argv[1]);
		Decompressor d(CompressedFileName);
		d.printDecompressInfo();
	}
	else if (argc == 3 && string(argv[1]) == "-d"){
		cout << "\n\tDecompressing, it may take some long time for big size file!" << endl;
		string CompressedFileName = string(argv[2]);
		Decompressor d(CompressedFileName);
		d.printDecompressInfo();
	}
	else if (string(argv[1]) == "-e"){
		for (int i = 3; i < argc; i++){
			filename.push_back(string(argv[i]));
		}
		cout << "\n\tCompressing, it may take some time for big size files!" << endl;
		string DecompressFileName = string(argv[2]);
		Compressor c(filename, DecompressFileName);
		c.printCompressInfo();
	}
	else{
		for (int i = 1; i < argc; i++){
			filename.push_back(string(argv[i]));
		}
		cout << "\n\tCompressing, it may take some time for big size files!" << endl;
		string DecompressFileName = PathUtil::extractFileName(filename[0]) + ".huf";
		Compressor c(filename, DecompressFileName);
		c.printCompressInfo();
	}
	cout << "\tPress any key to continue..." << endl;
	cin.get();
}
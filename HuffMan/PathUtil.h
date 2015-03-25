#pragma once
char separator;
#ifdef _WIN32
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR  '/'
#endif

#include <string>
#include <algorithm>
using namespace std;
class PathUtil{
public:
	/*A small function to extract FileName from File Path
	C:/File.exe -> File.exe
	*/
	string static extractFileName(string FilePath){

		int separatorlast = FilePath.find_last_of(DIR_SEPARATOR);
		return FilePath.substr(separatorlast + 1, FilePath.length() - separatorlast);
	}
	string static extractFileExt(string FilePath){
		int separatorlast = FilePath.find_last_of('.');
		string FileExt =  FilePath.substr(separatorlast + 1, FilePath.length() - separatorlast);
		std::transform(FileExt.begin(), FileExt.end(), FileExt.begin(), ::tolower);
		return FileExt;
	}
	string static extractFileNamewithoutExt(string FilePath){
		string FileName = extractFileName(FilePath);
		int separatorlast = FilePath.find_last_of(DIR_SEPARATOR);
		int dotlast = FilePath.find_last_of('.');
		return FilePath.substr(separatorlast + 1, dotlast);
	}
	string static extractDirPath(string FilePath){
		string FileName = extractFileName(FilePath);
		int separatorlast = FilePath.find_last_of(DIR_SEPARATOR);
		int dotlast = FilePath.find_first_of('.');
		return FilePath.substr(separatorlast + 1, dotlast - separatorlast -1);
	}
};
#pragma once
#include <queue>
using namespace std;
class BitQueue{
	queue<bool>* bitqueue;
public:
	
	void reset(){
		delete bitqueue;
		this->bitqueue = new queue<bool>;
	}
	BitQueue(){
		this->bitqueue = new queue <bool>;
	}
	BitQueue(istream& ByteStream){
		this->bitqueue = new queue <bool>;
		while (!ByteStream.eof()){
			(*this)<< (unsigned char)ByteStream.get();
		}
	}
	BitQueue(const BitQueue& copy){
		this->bitqueue = new queue<bool>(*copy.bitqueue);
	}
	~BitQueue(){
		delete bitqueue;
	}
	int bitsize(){
		return bitqueue->size();
	}
	int size(){
		return static_cast<int>(ceil(bitqueue->size() / (double)8));
	}
	/*A single bit*/
	BitQueue& operator<<(bool a){
		bitqueue->push(a);
		return *this;
	}
	/*A byte*/
	BitQueue& operator<<(unsigned char a){
		//string BinaryString = "";
		for (int i = 7; i != -1;i--){
		bool bitat = (a & (1 << i)) == 0 ? false : true;
		operator<<(bitat);
		//BinaryString = string(1,mod + '0') + BinaryString;
		}
		return *this;
	}
	BitQueue& operator<<(const string binaryString){
		for (unsigned int i = 0; i < binaryString.length(); i++){
			bitqueue->push(binaryString[i] - '0' == 0 ? false : true);
		}
		return *this;
	}
	bool empty(){ return bitqueue->empty(); }
	BitQueue& operator>>(bool& bit){
		bit = this->bitqueue->front();
		this->bitqueue->pop();
		return *this;
	}
	BitQueue& operator>>(unsigned char& a){
		if (this->bitqueue->empty()) {
			a = -1;
			return *this;
		}
		a = 0;
		for (int i = 7; i != -1; i--){
			if (empty()) return *this;
			bool bit = this->bitqueue->front();
				if (bit) a |= (1 << i);
			this->bitqueue->pop();
		}
		/*a = 0;
		int k = 128;
		for (int i = 0; i < 8; i++){
			if (empty()) return *this;
			bool bit = this->bitqueue->front();
			this->bitqueue->pop();
			a += k*bit;
			k /= 2;
		}*/
		return *this;
	}
	string toString(){
		queue<bool> copied(*bitqueue);
		char* str = new char[bitsize()+1];
		for (int i = 0; i < bitsize(); i++){
			str[i] = copied.front() + '0';
			copied.pop();
		}
		str[bitsize()] = 0;
		string String(str);
		delete[] str;
		return String;
	}
};

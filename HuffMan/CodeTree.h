#pragma once
#include <string>
#include <istream>
#include <vector>
#include "Node.h"
#include "InternalNode.h"
#include "LeafNode.h"
#include "BitQueue.h"
using namespace std;
class CodeTree{
	vector<Node*> nodesList;
	int n = -1;
	Node* getCode(unsigned char symbol, Node* root, string& key){
		Node* parent = root;
		if (root == NULL) return NULL;
		if (parent->left() != NULL && typeid(*parent->left()) == typeid(LeafNode) && parent->left()->symbol() == symbol){
			key.push_back('0');
			return parent->left();
		}
		if (parent->right() != NULL && typeid(*parent->right()) == typeid(LeafNode) && parent->right()->symbol() == symbol){
			key.push_back('1');
			return parent->right();
		}
		if (typeid(parent->left()) == typeid(LeafNode) && typeid(parent->right()) == typeid(LeafNode)) return NULL;
		key.push_back('0');
		Node* LeftRecursion = getCode(symbol, parent->left(), key);
		if (LeftRecursion != NULL)
		{
			return LeftRecursion;
		}
		key.pop_back();
		key.push_back('1');
		Node* RightRecursion = getCode(symbol, parent->right(), key);	
		if (RightRecursion != NULL){
			return RightRecursion;
		}
		key.pop_back();
		return NULL;
	}
public:
	CodeTree(){
	}
	//Construct HuffmanTree from string
	CodeTree(istream& stream): codeBook(){
		/*Reset stream*/
		stream.clear();
		stream.seekg(0, ios::beg);
		put(stream);
		build();
	}
	/*Reconstruct HuffmanTree from a root Node*/
	CodeTree(Node* root){
		this->nodesList.push_back(root);
	}
	~CodeTree(){
	}
	string getCode(unsigned char symbol){
		if (codeBook[symbol] == ""){
		getCode(symbol, nodesList[0], codeBook[symbol]);
		}
		return codeBook[symbol];
	}
	int getSymbol(BitQueue& BitQueue){
			Node* mNode = getRoot();
			bool bit = 0;
			while (!BitQueue.empty()){
				BitQueue >> bit;
				if (bit == 0){
					mNode = mNode->left();
				}
				else if (bit == 1){
					mNode = mNode->right();
				}
				else{
					throw "Invaild key string";
				}
				if (typeid(*mNode) == typeid(LeafNode)) return mNode->symbol();
			}
			//No symbol found
			return -1;
	}
	void put(istream& stream){
		unsigned char c = 0;
		while (!stream.eof()){
			stream >> noskipws >> c;
			//search char
			put(c);
		}
	}
	void put(unsigned char c){
		bool found = false;
		//search char
		for (unsigned int j = 0; j < nodesList.size(); j++){
			if (c == nodesList[j]->symbol()){
				nodesList[j]->incFreq();
				found = true;
				break;
			}
		}
		if (found) return;
		//symbol is not available
		LeafNode* newNode = new LeafNode(c);
		newNode->incFreq();
		nodesList.push_back(newNode);
	}
	vector<int> getCodeLengths(){
		vector<int> codeLengths;
		codeLengths.resize(256);
		for (int i = 0; i < 256; i++){
			string key = getCode(i);
			if (key != ""){
				codeLengths[i] = key.length();
			}
			else{
				codeLengths[i] = 0;
			}
		}
		return codeLengths;
	}
	void build(){
		if (nodesList.size() == 1) nodesList.push_back(new InternalNode);
		while (nodesList.size() != 1){
			Node* Child[2];
			int minFreqIndex[2] = { 0, 0 };
			int minFreq[2] = { nodesList[minFreqIndex[0]]->freq(), nodesList[minFreqIndex[0]]->freq() };
			for (int i = 0; i < 2; i++){
				for (unsigned int j = 1; j < nodesList.size(); j++){
					if (nodesList[j]->freq() <= minFreq[i]){
						minFreq[i] = nodesList[j]->freq();
						minFreqIndex[i] = j;
					}
				}
				Child[i] = nodesList[minFreqIndex[i]];
				nodesList.erase(nodesList.begin() + minFreqIndex[i]);
			}
			Node* newNode = new InternalNode;
			newNode->left(Child[0]);
			newNode->right(Child[1]);
			newNode->freq(newNode->left()->freq() + newNode->right()->freq());
			nodesList.push_back(newNode);
		}
	}
	Node* getRoot(){ return nodesList[0]; }
	int countSymbol(){
		if (n == -1){
		n = 0;
		vector<int> CodeLengths = this->getCodeLengths();
		for (unsigned int i = 0; i < CodeLengths.size(); i++){
			if (CodeLengths[i] != 0) n++;
		}
		}
		return n;
	}
	private:
		/*Use for cache purpose*/
		string codeBook[256];
};
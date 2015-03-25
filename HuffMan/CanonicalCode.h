#pragma once
#include "CodeTree.h"
class CanonicalCode{
	bool onechar = false;
	vector<vector<Node*>> DepthBasedLeafList;
	vector<vector<Node*>> DepthBasedIntenalList;
	vector<Node*> codeTree;
	void addNode(Node* node, int depth){
		if (node == NULL) return;
		if (typeid(*node) == typeid(InternalNode)){
			DepthBasedIntenalList[depth].push_back(node);
		}
		else{
			DepthBasedLeafList[depth].push_back(node);
		}
	}
	bool addLeftMost(Node* node, Node* parent, int depth){
		if (typeid(*parent) == typeid(LeafNode)) return 0;
		if (depth == 1){
			if (parent->left() == NULL){
				(parent->left(node));
				return 1;
			}
			if (parent->right() == NULL){
				(parent->right(node));
				return 1;
			}
			return 0;
		}
		else{
			//If Left Node is avaiable, add it.
			if (parent->left() == NULL){
				parent->left(new InternalNode);
				if (addLeftMost(node, parent->left(), depth - 1)) return 1;
			}
			if (typeid(*parent->left()) == typeid(InternalNode)){
				if (addLeftMost(node, parent->left(), depth - 1)) return 1;
			}
			if (parent->right() == NULL){
				parent->right(new InternalNode);
				if (addLeftMost(node, parent->right(), depth - 1)) return 1;
			}

			if (typeid(*parent->right()) == typeid(InternalNode)){
				if (addLeftMost(node, parent->right(), depth - 1)) return 1;
			}
		}
		return 0;
	}
public:
	CanonicalCode(CodeTree& tree){
		DepthBasedLeafList.resize(256);
		DepthBasedIntenalList.resize(256);
		buildDepthList(tree);
	}
	CanonicalCode(vector<int> codeLengths){
		DepthBasedLeafList.resize(256);
		DepthBasedIntenalList.resize(256);
		buildDepthList(codeLengths);
	}
	~CanonicalCode(){}
	CodeTree buildCodeTree(){
		Node* root =  new InternalNode();
		for (unsigned int depth = 1; depth < DepthBasedLeafList.size(); depth++){
			if (DepthBasedLeafList[depth].empty()) continue;
			for (unsigned int i = 0; i < DepthBasedLeafList[depth].size(); i++){
				addLeftMost(DepthBasedLeafList[depth][i], root, depth);
			}
		}
		return CodeTree(root);
		//return *(new CodeTree(root));
	}
	void buildDepthList(CodeTree& tree){
		DepthBasedIntenalList[0].push_back(tree.getRoot());
		for (int depth = 1; depth < 256; depth++){
			if (DepthBasedIntenalList[depth - 1].size() == 0) break;
			for (unsigned int i = 0; i < DepthBasedIntenalList[depth - 1].size(); i++){
				Node* pLeftNode = DepthBasedIntenalList[depth - 1][i]->left();
				Node* pRightNode= DepthBasedIntenalList[depth - 1][i]->right();
				//Left Node
				addNode(pLeftNode, depth);
				addNode(pRightNode, depth);
				
			}
		}
		SortDepthBasedLeafList();
	}
	void buildDepthList(vector<int> codeLengths){
		for (unsigned int i = 0; i < codeLengths.size(); i++){
			if (codeLengths[i] == 0) continue;
			DepthBasedLeafList[codeLengths[i]].push_back(new LeafNode(i));
		}
	}
	void SortDepthBasedLeafList(){
		for (unsigned int depth = 0; depth < DepthBasedLeafList.size(); depth++){
			std::sort(DepthBasedLeafList[depth].begin(),DepthBasedLeafList[depth].end(), Node::cmp);
		}
	}
};
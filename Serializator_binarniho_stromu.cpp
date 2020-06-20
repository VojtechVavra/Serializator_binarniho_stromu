// Serializator_binarniho_stromu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>	// std::sort
#include <vector>
#include <fstream>

using namespace std;

struct TreeNode
{
	int number;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;

	TreeNode() {};
	TreeNode(int n) : number(n) {};
};

void GetRandomSequence(int, int*);
TreeNode* GetLeftNode(vector<int>);
TreeNode* GetRightNode(vector<int>);
TreeNode* CreateNode(vector<int>);

string Serialize(TreeNode*, bool);
TreeNode* DeSerialize(string);
TreeNode* CreateNode(TreeNode*, vector<string>&);
void SaveToFile(string, string);

int main()
{
	srand(time(nullptr));

	int N = rand() % 7 + 1;
	int* pole = new int[N];

	cout << "Random numbers: " << N << endl;
	cout << "Unique random number sequence" << endl;
	GetRandomSequence(N, pole);
	sort(pole, pole + N);
	cout << "Sorted sequence" << endl;
	for (int i = 0; i < N; i++)
	{
		cout << pole[i] << " ";
	}
	cout << endl << endl;

	vector<int> vectVal;
	for (int i = 0; i < N; i++)
	{
		vectVal.push_back(pole[i]);
	}

	// serialize from random number tree
	cout << "Serialized outuput from random tree" << endl;
	TreeNode* tr = CreateNode(vectVal);
	string serializedOutput = Serialize(tr, true);
	cout << serializedOutput << endl << endl;


	// deserialize from file and serialize into another file
	TreeNode* newTree = DeSerialize("input.txt");  // load from file
	string serializedOutput2 = Serialize(newTree, true);
	cout << "Serialized output" << endl;
	cout << serializedOutput2 << endl;
	SaveToFile(serializedOutput2, "output.txt");

	
	delete[] pole;

    return 0;
}

void GetRandomSequence(int N, int* pole)
{
	int i = 0;
	int randomNumber;
	bool isOnce;

	while (i < N)
	{
		randomNumber = rand() % 20;
		isOnce = true;

		if (i == 0) {
			pole[i] = randomNumber;
			cout << pole[i] << " ";
			i++;
			continue;
		}
		else
		{
			for (int j = 0; j < i; j++)
			{
				if (pole[j] == randomNumber)
				{
					isOnce = false;
					break;
				}
			}
			if (isOnce) {
				pole[i] = randomNumber;
				cout << pole[i] << " ";
				i++;
			}
		}
	}
	cout << endl;
}

TreeNode* GetLeftNode(vector<int> vect1)
{
	if (vect1.size() == 0)
		return nullptr;

	int stred = vect1.size() / 2;
	TreeNode* node = new TreeNode(vect1[stred]);
	vect1.erase(vect1.begin() + stred);

	vector<int> vectL;
	vector<int> vectR;

	for (int i = 0; i < stred; i++)
		vectL.push_back(vect1[i]);
	for (int i = stred; i < vect1.size(); i++)
		vectR.push_back(vect1[i]);

	if (vectL.size() == 0)
		node->left = nullptr;
	else
		node->left = GetLeftNode(vectL);

	if (vectR.size() == 0)
		node->right = nullptr;
	else
		node->right = GetRightNode(vectR);
	
	return node;
}

TreeNode* GetRightNode(vector<int> vect1)
{
	if (vect1.size() == 0)
		return nullptr;

	int stred = vect1.size() / 2;
	TreeNode* node = new TreeNode(vect1[stred]);
	vect1.erase(vect1.begin() + stred);

	vector<int> vectL;
	vector<int> vectR;

	for (int i = 0; i < stred; i++)
		vectL.push_back(vect1[i]);
	for (int i = stred; i < vect1.size(); i++)
		vectR.push_back(vect1[i]);

	if (vectL.size() == 0)
		node->left = nullptr;
	else
		node->left = GetLeftNode(vectL);

	if (vectR.size() == 0)
		node->right = nullptr;
	else
		node->right = GetRightNode(vectR);

	return node;
}

TreeNode* CreateNode(vector<int> vectNum)
{
	int stred = (vectNum.size() - 1) / 2;
	TreeNode* firstNode = new TreeNode(vectNum[stred]);

	vectNum.erase(vectNum.begin() + stred);

	vector<int> vectL(vectNum.begin(), vectNum.begin() + vectNum.size() / 2);
	vector<int> vectR(vectNum.begin() + vectNum.size() / 2, vectNum.end());

	firstNode->left = GetLeftNode(vectL);
	firstNode->right = GetRightNode(vectR);

	return firstNode;
}

string Serialize(TreeNode* mainNode, bool del = false)
{
	static string serializedTree;
	if (del)
		serializedTree = "";

	serializedTree += to_string(mainNode->number) + " ";

	if (mainNode->left == nullptr)
	{
		serializedTree += "x ";
	}
	if (mainNode->left != nullptr)
	{
		Serialize(mainNode->left);
	}
	if (mainNode->right == nullptr)
	{
		serializedTree += "x ";
	}
	if (mainNode->right != nullptr)
	{
		Serialize(mainNode->right);
	}

	return serializedTree;
}

TreeNode* DeSerialize(string inputFilePath)
{
	ifstream file;
	file.open(inputFilePath);
	string inputSerializedTreeTxt = inputFilePath;

	TreeNode* newTree = new TreeNode;
	string num;
	vector<string> values;
	if (file.is_open()) {
		cout << "File input" << endl;
		while (file >> num)
		{
			values.push_back(num);
			cout << num << " ";
		}
	}
	file.close();

	cout << endl;
	newTree = CreateNode(newTree, values);

	return newTree;
}

TreeNode* CreateNode(TreeNode* prevNode, vector<string>& strNumbers)
{
	if (strNumbers.size() == 0)
		return prevNode;
	if (strNumbers[0] == "x")
	{
		strNumbers.erase(strNumbers.begin());
		return nullptr;
	}
		
	TreeNode* newNode = new TreeNode;
	newNode->number = stoi(strNumbers[0]);
	strNumbers.erase(strNumbers.begin());

	newNode->left = CreateNode(newNode, strNumbers);

	if (strNumbers.size() > 0 && strNumbers[0] == "x")
	{
		strNumbers.erase(strNumbers.begin());
		newNode->right = nullptr;
		return newNode;
	}

	if (stoi(strNumbers[0]) > newNode->number)
	{
		newNode->right = CreateNode(newNode, strNumbers);
	}

	return newNode;
}

void SaveToFile(string serializedTree, string outputFileName)
{
	ofstream serFile(outputFileName);

	if (serFile.is_open())
	{
		serFile << serializedTree;
		serFile.close();
	}
	else
		cout << "Unable to open file";
}

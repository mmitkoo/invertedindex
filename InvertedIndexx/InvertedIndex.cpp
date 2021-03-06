#include "dirent.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <stack>
using namespace std;

const string directory = "d:\\src\\";

struct invertedIndex
{
	string word;
	list <int> indexes;
	
	invertedIndex(string word)
	{
		this->word = word;
	}

	void print()
	{
		cout << "Word: " << word << " Indexes: ";
		for (int in : indexes)
		{
			cout << in << " ";
		}
		cout << endl;
	}
};

vector<string> listFileNames(const string& dir_name)
{
	vector<string> names;
	DIR *dir;
	dirent *ent;
	dir = opendir(dir_name.c_str());
	assert(dir);
	ent = readdir(dir);
	assert(ent);
	while (ent)
	{
		if (ent->d_name[0] != '.')
		{
			names.push_back(ent->d_name);
		}
		ent = readdir(dir);
	}
	closedir(dir);
	return names;
}

string toLowerCase(string& str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] = str[i] + ('a' - 'A');
		}
	}
	return str;
}

void updateVector(vector<invertedIndex>& words, string word, int index)
{
	if (words.empty() || word > words.back().word)
	{
		invertedIndex temp(word);
		temp.indexes.push_back(index);
		words.push_back(temp);
		return;
	}

	int left = 0;
	int right = words.size() - 1;
	int middle;
	while (left <= right)
	{
		middle = (left + right) / 2;
		if (words[middle].word == word)
		{
			for (int ind : words[middle].indexes)
			{
				if (ind == index)
				{
					return;
				}
			}
			words[middle].indexes.push_back(index);
			return;
		}
		if (word < words[middle].word)
		{
			right = middle - 1;
		}
		else
		{
			left = middle + 1;
		}
	}
	if (right < left)
	{
		invertedIndex temp(word);
		temp.indexes.push_back(index);
		auto it = words.begin();
		for (int i = 0; i < left; i++)
		{
			it++;
		}
		words.insert(it,temp);
	}
}

vector<invertedIndex> listingWords(const string& dir_name)
{
	vector<string> fileNames = listFileNames(dir_name);
	int ind = 0;
	vector<invertedIndex> words;
	ifstream fin;
	string word;
	for (string v : fileNames)
	{
		fin.open(dir_name + v);
		while (!fin.eof())
		{
			fin >> word;
			updateVector(words, toLowerCase(word), ind);
		}
		fin.close();
		ind++;
	}
	
	return words;
}

list<int> simpleQueries(const string& word)
{
	vector<invertedIndex> words = listingWords(directory);
	int left = 0;
	int right = words.size() - 1;
	int middle;
	while (left <= right)
	{
		middle = (left + right) / 2;
		if (words[middle].word == word)
		{
			break;
		}
		if (word < words[middle].word)
		{
			right = middle - 1;
		}
		else
		{
			left = middle + 1;
		}
	}

	if (words[middle].word != word)
	{
		list<int> lst;
		return lst;//Празен лист
	}
	else
	{
		return words[middle].indexes;
	}
}

void printFileNames(const string& word)
{

	vector<string> fileNames = listFileNames(directory);
	list<int> lst = simpleQueries(word);
	cout << word << " -> ";
	for (int ind : lst)
	{
		if (ind == lst.back())
		{
			cout << fileNames[ind] << endl;
			return;
		}
		cout << fileNames[ind] << ", ";
	}
}

list<int> operator+ (const list<int>& wordIndexes1,const list<int>& word2Indexes2)
{
	if (wordIndexes1.empty())
	{
		return word2Indexes2;
	}

	if (word2Indexes2.empty())
	{
		return wordIndexes1;
	}

	list<int> result;
	auto it1 = wordIndexes1.cbegin(), it2 = word2Indexes2.cbegin();
	while (it1 != wordIndexes1.cend() && it2 != word2Indexes2.cend())
	{
		if (*it1 == *it2)
		{
			result.push_back(*it1);
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			result.push_back(*it1);
			++it1;
		}
		else
		{
			result.push_back(*it2);
			++it2;
		}
	}

	for (; it1 != wordIndexes1.cend(); ++it1)
	{
		result.push_back(*it1);
	}

	for (; it2 != word2Indexes2.cend(); ++it2)
	{
		result.push_back(*it2);
	}

	return result;
}

list<int> operator*(const list<int>& wordIndexes1, const list<int>& word2Indexes2)
{
	if (wordIndexes1.empty() || word2Indexes2.empty())
	{
		return {};
	}
	list <int> result;
	auto it1 = wordIndexes1.cbegin(), it2 = word2Indexes2.cbegin();
	while (it1 != wordIndexes1.cend() && it2 != word2Indexes2.cend())
	{
		if (*it1 == *it2)
		{
			result.push_back(*it1);
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			++it1;
		}
		else
		{
			++it2;
		}
	}

	return result;
}

list<int> operator-(const list<int>& wordIndexes1, const list<int>& word2Indexes2)
{
	if (wordIndexes1.empty())
	{
		return {};
	}

	if (word2Indexes2.empty())
	{
		return wordIndexes1;
	}
	list<int> result;
	auto it1 = wordIndexes1.cbegin(), it2 = word2Indexes2.cbegin();
	while (it1 != wordIndexes1.cend() && it2 != word2Indexes2.cend())
	{
		if (*it1 == *it2)
		{
			++it1;
			++it2;
		}
		else if (*it1 < *it2)
		{
			result.push_back(*it1);
			++it1;
		}
		else
		{
			++it2;
		}
	}

	for (; it1 != wordIndexes1.cend(); ++it1)
	{
		result.push_back(*it1);
	}

	return result;
}

size_t getPrecedence(char op)
{
	switch (op)
	{
	case '+': return 1;
	case '-': return 2;
	case '*': return 3;
	case '(': return 100;
	}
}

string convertIntoPolish(const string& expression)
{
	istringstream expr(expression);
	stack<char> st;
	ostringstream result;
	string word;
	char symbol;
	while (!expr.eof())
	{
		char next = expr.peek();
		if (next == '+' || next == '-' || next == '*')
		{
			expr >> symbol;
			size_t precedence = getPrecedence(symbol);
			while (!st.empty() &&
				getPrecedence(st.top()) >= precedence &&
				st.top() != '(')
			{
				result << st.top() << " ";
				st.pop();
			}
			st.push(symbol);
		}
		else if (next >= 'a' && next <= 'z')
		{
			expr >> word;
			result << word << " ";
		}
		else if (next == '(')
		{
			expr >> symbol;
			st.push(symbol);
		}
		else if (next == ')')
		{
			expr >> symbol;
			while (st.top() != '(')
			{
				result << st.top() << " ";
				st.pop();
			}
			st.pop();
		}
		else
		{
			expr.get();
		}
	}

	while (!st.empty()) {
		result << st.top();
		st.pop();
	}

	return result.str();
}

list<int> advancedQueries(const string& expression)
{
	istringstream expr(convertIntoPolish(expression));
	stack<list<int>> st;
	char operation;
	list<int> list1, list2;
	string word;
	while (!expr.eof())
	{
		char next = expr.peek();
		if (next == '+' || next == '-' || next == '*')
		{
			expr >> operation;
			switch (operation)
			{
			case '+':
				list2 = st.top();
				st.pop();
				list1 = st.top();
				st.pop();
				st.push(list1 + list2);
				break;
			case '-':
				list2 = st.top();
				st.pop();
				list1 = st.top();
				st.pop();
				st.push(list1 - list2);
				break;
			case '*':
				list2 = st.top();
				st.pop();
				list1 = st.top();
				st.pop();
				st.push(list1 * list2);
				break;
			}
		}
		else if (next >= 'a' && next <= 'z')
		{
			expr >> word;
			st.push(simpleQueries(word));
		}
		else
		{
			expr.get();
		}
	}

	return st.top();
}

void printAdvancedQueries(const string& expression)
{
	list<int> indexes = advancedQueries(expression);
	vector<string> fileNames = listFileNames(directory);
	cout << expression << " -> ";
	if (indexes.empty())
	{
		cout << "No Results Found" << endl;
		return;
	}
	for (int ind : indexes)
	{
		if (ind == indexes.back())
		{
			cout << fileNames[ind] << endl;
			return;
		}
		cout << fileNames[ind] << ", ";
	}
	cout << endl;
}

int main()
{
	string str;
	cout << "Start entering queries: "<<endl;
	while (getline(cin,str))
	{
		printAdvancedQueries(toLowerCase(str));
	}
	return 0;
}
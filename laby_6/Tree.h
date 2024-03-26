#pragma once
#include "Node.h"

class Tree
{
	Node* root;

public:
	Tree()
	{
		root = new Node();
	}

	Node* findNode( std::string word )
	{
		Node* current = root;

		for (int i = 0; i < word.length(); i++)
		{
			int letter = word[i] - 'a';
			if (!current->children[letter])
			{
				return nullptr;
			}

			current = current->children[letter];
		}

		return current;
	}

	void addTranslation( std::string word, std::string translation )
	{
		Node* current = root;

		for (int i = 0; i < word.length(); i++)
		{
			int letter = word[i] - 'a';
			if (!current->children[letter])
			{
				current->children[letter] = new Node();
			}

			current = current->children[letter];
		}

		current->translation = translation;
	}

	void printAllWithPrefix( std::string prefix )
	{
		Node* start = findNode( prefix );
		if (!start)
		{
			std::cout << '-' << std::endl;
			return;
		}

		dfs( start );
	}

	void dfs( Node* node )
	{
		if (node->translation != "-")
			std::cout << node->translation << std::endl;

		for (int i = 0; i < AlphSize; i++)
		{
			if (node->children[i])
			{
				dfs( node->children[i] );
			}
		}
	}

	void deleteNodes( Node* node )
	{
		for (int i = 0; i < AlphSize; i++)
		{
			if (node->children[i])
			{
				deleteNodes( node->children[i] );
			}
		}

		delete node;
	}

	~Tree()
	{
		deleteNodes( root );
	}
};


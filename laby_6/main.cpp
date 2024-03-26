#include <iostream>
#include <string>

#include "Tree.h"
#include "Node.h"

using namespace std;

int main()
{
	char command;
	Tree tree;

	while (cin >> command)
	{
		string word = "", translation = "";
		Node* node;

		switch (command)
		{
		case '+':
			cin >> word >> translation;
			tree.addTranslation( word, translation );
			break;
		case '?':
			cin >> word;
			node = tree.findNode( word );
			if (node)
			{
				cout << node->translation << endl;
			}
			else
			{
				cout << '-' << endl;
			}
			break;
		case '*':
			cin >> word;
			tree.printAllWithPrefix( word );
			break;
		}
	}

	return 0;
}
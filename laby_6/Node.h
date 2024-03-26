#pragma once
#include <iostream>
#include <string>

constexpr int AlphSize = 26;

class Node
{
public:
	Node* children[AlphSize];
	std::string translation = "-";

	Node()
	{
		for (int i = 0; i < AlphSize; i++)
		{
			children[i] = nullptr;
		}
	}
};


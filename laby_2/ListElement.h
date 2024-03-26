#pragma once
class ListElement
{
public:
	int key;
	ListElement* next = nullptr;

	ListElement( int key, ListElement* next );
};


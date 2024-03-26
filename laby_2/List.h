#pragma once

#include "ListElement.h"

class List
{
private:
	unsigned long long int length = 0;
	ListElement* head = nullptr;

public:
	void dropElement( int index );
	void addElement( int key, int index );
	void deleteList();
	int getElementKey( int index ) const;
	unsigned long long int getLength() const;
};
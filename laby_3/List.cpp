#include "List.h"

void List::dropElement( int index )
{
	if ( index == 0 )
	{
		if ( head->next != nullptr ) head = head->next;
		else head = nullptr;
		length--;
		return;
	}

	ListElement* current = head, * previous = nullptr;

	for ( int i = 1; i <= index; i++ )
	{
		previous = current;
		current = current->next;
	}

	if( previous != nullptr ) previous->next = current->next;
	delete current;

	length--;
}

void List::addElement( int key, int index )
{
	if ( index == 0 )
	{
		/*if ( head != nullptr && head->next != nullptr ) head = new ListElement( key, head );
		else head = new ListElement( key, nullptr );*/
		head = new ListElement( key, head );
		length++;
		return;
	}

	ListElement* current = head;

	for ( int i = 1; i < index; i++ )
	{
		current = current->next;
	}

	current->next = new ListElement( key, current->next );

	length++;
}

void List::deleteList()
{
	ListElement* current = head, * previous = nullptr;

	while ( current != nullptr )
	{
		previous = current;
		current = current->next;
		delete previous;
	}
}

int List::getElementKey( int index ) const
{
	ListElement* current = head;

	for ( int i = 1; i <= index; i++ )
	{
		current = current->next;
	}

	return current->key;
}

unsigned long long int List::getLength() const
{
	return length;
}

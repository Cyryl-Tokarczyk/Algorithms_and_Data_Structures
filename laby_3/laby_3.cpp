#include <iostream>

#include "List.h"
#include "ListElement.h"

using namespace std;

void sort( List* numbers, int n );
bool isGreater( List first, List second );

int main()
{
	int n;
	cin >> n;
	cin.ignore();

	List* numbers = new List[n];

	for ( int i = 0; i < n; i++ )
	{
		char input;
		input = cin.get();

		while ( input != '\n' )
		{
			numbers[i].addElement( input, numbers[i].getLength() );
			input = cin.get();
		}
	}

	sort( numbers, n );

	for ( int i = 0; i < n; i++ )
	{
		for ( int j = 0; j < numbers[i].getLength(); j++ )
		{
			cout << (char)numbers[i].getElementKey( j );
		}
		cout << endl;
	}

	delete[] numbers;

	return 0;
}

void sort( List* numbers, int n )
{
	for ( int i = 1; i < n; i++ )
	{
		List current = numbers[i];
		int j = i - 1;
		while ( j >= 0 && isGreater( numbers[j], current) )
		{
			numbers[j + 1] = numbers[j];
			j -= 1;
		}
		numbers[j + 1] = current;
	}
}

bool isGreater( List first, List second )
{
	// Porownac znak
	// Porownac Dlugosc
	// Porownywac elementy dopoki sie nie roznia

	bool firstIsPos = first.getElementKey(0) == '-' ? false : true, secondIsPos = second.getElementKey( 0 ) == '-' ? false : true;

	if ( (firstIsPos && !secondIsPos) || (!firstIsPos && secondIsPos) )
	{
		if ( firstIsPos )
			return true;
		else
			return false;
	}

	if ( first.getLength() != second.getLength() )
	{
		if ( firstIsPos )
		{
			if ( first.getLength() > second.getLength() )
				return true;
			else
				return false;
		}
		else
		{
			if ( first.getLength() < second.getLength() )
				return true;
			else
				return false;
		}
	}

	if ( firstIsPos )
	{
		for ( int i = 0; i < first.getLength(); i++ )
		{
			char f = first.getElementKey( i ), s = second.getElementKey( i );
			if ( f != s )
			{
				if ( f > s )
					return true;
				else
					return false;
			}
		}
	}
	else
	{
		for ( int i = 0; i < first.getLength(); i++ )
		{
			char f = first.getElementKey( i ), s = second.getElementKey( i );
			if ( f != s )
			{
				if ( f < s )
					return true;
				else
					return false;
			}
		}
	}

	return false;
}
#include <iostream>
#include "List.h"

using namespace std;

int main()
{
	char command;
	List list;

	while ( cin >> command )
	{
		int k, i;

		switch ( command )
		{
		case 'i':
			cin >> k >> i;
			list.addElement( k, i );
			break;
		case 'd':
			cin >> i;
			list.dropElement( i );
			break;
		case 'g':
			cin >> i;
			cout << list.getElementKey( i ) << endl;
			break;
		case 'q':
			list.deleteList();
			return 0;
		default:
			cout << "Improper command";
			break;
		}
	}

	list.deleteList();

	return 0;
}
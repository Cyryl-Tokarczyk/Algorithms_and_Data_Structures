#include <iostream>
#include <string>

constexpr int MaxSize = 100000;

using namespace std;

struct Element
{
	string key;
	int count;
};

int hashString( string toHash );
void find( string toFind, Element** table );
void remove( string toRemove, Element** table );
void insert( string& toInsert, Element** table );

int main()
{
	Element** table;
	table = new Element * [MaxSize];
	for (int i = 0; i < MaxSize; i++)
	{
		table[i] = nullptr;
	}

	string command;

	while (cin >> command)
	{
		if (command[0] == '?')
		{
			find( command, table );
		}
		else if (command[0] == '-')
		{
			remove( command, table );
		}
		else if (command.length() == 1 && command[0] == 'q')
		{
			break;
		}
		else
		{
			insert( command, table );
		}
	}

	for (int i = 0; i < MaxSize; i++)
	{
		if (table[i])
			delete table[i];
	}

	delete[] table;

	return 0;
}

int hashString( string toHash )
{
	uint64_t hash = 0;
	for (int i = 0; i < toHash.length(); i++)
	{
		hash += toHash[i];
	}
	hash *= toHash[0];
	return hash % 100000;
}

void find( string toFind, Element** table )
{
	toFind.erase( toFind.begin() );

	int hash = hashString( toFind );
	if (table[hash] && table[hash]->key != toFind)
	{
		for (int i = hash; table[i]; i++)
		{
			if (table[i]->key == toFind)
			{
				cout << table[i]->count << endl;
				return;
			}
			if (i + 1 >= MaxSize)
				i = -1;
		}

		cout << 0 << endl;
		return;
	}
	else if (table[hash])
	{
		cout << table[hash]->count << endl;
	}
	else
	{
		cout << 0 << endl;
	}
}

void remove( string toRemove, Element** table )
{
	toRemove.erase( toRemove.begin() );

	int hash = hashString( toRemove );

	if (!table[hash])
		return;

	if (table[hash]->key != toRemove)
	{
		for (int i = hash; table[i]; i++)
		{
			if (table[i]->key == toRemove)
			{
				table[i]->count = 0;
				return;
			}
			if (i + 1 >= MaxSize)
				i = -1;
		}

		return;
	}

	table[hash]->count = 0;
}

void insert( string& toInsert, Element** table )
{
	int hash = hashString( toInsert );

	if (table[hash])
	{
		if (table[hash]->key == toInsert)
		{
			table[hash]->count++;
			return;
		}
		else
		{
			// Open address mapping

			int i;
			for (i = hash; table[i]; i++)
			{
				if (table[i]->key == toInsert)
				{
					table[i]->count++;
					return;
				}
				if (i + 1 >= MaxSize)
					i = -1;
			}

			Element* element = new Element;
			element->key = toInsert;
			element->count = 1;

			table[i] = element;

			return;
		}
	}

	Element* element = new Element;
	element->key = toInsert;
	element->count = 1;

	table[hash] = element;

	return;
}

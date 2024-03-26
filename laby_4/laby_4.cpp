#include <iostream>
#include <string>
#include <vector>

using namespace std;

int parent( int i );
int right( int i );
int left( int i );
void heapify( string* arr, int i, int size, bool isMaxHeap );
bool isGreater( string first, string second );
void buildHeap( string* arr, int size, bool isMaxHeap );

int main()
{
	bool isMaxHeap = true;
	char command;
	vector<string> heap;

	while ( cin >> command )
	{
		if ( command == '+' )
		{
			string num;
			cin >> num;
			heap.push_back( num );
			int child = heap.size() - 1, par = parent(child);
			if ( isMaxHeap )
			{
				while ( child > 0 && isGreater( heap[child], heap[par] ) )
				{
					swap( heap[child], heap[par] );
					child = par;
					par = parent( child );
				}
			}
			if ( !isMaxHeap )
			{
				while ( child > 0 && isGreater( heap[par], heap[child] ) )
				{
					swap( heap[child], heap[par] );
					child = par;
					par = parent( child );
				}
			}
		}
		if ( command == 'p' )
		{
			//int j = 0;
			for ( int i = 0; i < heap.size(); i++ )
			{
				std::cout << heap[i] << ' ';

				/*if ( i == j )
				{
					std::cout << std::endl;
					j = j == 0 ? 2 : j + j * 2;
				}*/
			}
			cout << endl;
		}
		if ( command == '-' )
		{
			cout << heap[0] << endl;
			heap[0] = heap[heap.size()-1];
			heap.pop_back();
			heapify( heap.data(), 0, heap.size(), isMaxHeap );
		}
		if ( command == 'r' )
		{
			int n;
			vector<string> newHeap;
			cin >> n;
			for ( int i = 0; i < n; i++ )
			{
				string num;
				cin >> num;
				newHeap.push_back( num );
			}
			buildHeap( newHeap.data(), newHeap.size(), isMaxHeap );
			heap.swap( newHeap );
		}
		if ( command == 's' )
		{
			isMaxHeap = !isMaxHeap;
			buildHeap( heap.data(), heap.size(), isMaxHeap );
		}
		if ( command == 'q' )
			exit;
	}

	return 0;
}

int parent( int i )
{
	return (i - 1) / 2;
}

int left( int i )
{
	return i * 2 + 1;
}

int right( int i )
{
	return i * 2 + 2;
}

void heapify( string* arr, int i, int size, bool isMaxHeap )
{
	int l = left( i );
	int r = right( i );
	int max = i;

	if ( isMaxHeap )
	{
		if ( l < size && isGreater( arr[l], arr[max] ) )
			max = l;

		if ( r < size && isGreater( arr[r], arr[max] ) )
			max = r;

		if ( max != i )
		{
			std::swap( arr[i], arr[max] );
			heapify( arr, max, size, isMaxHeap );
		}
	}
	else
	{
		if ( l < size && isGreater( arr[max], arr[l] ) )
			max = l;

		if ( r < size && isGreater( arr[max], arr[r] ) )
			max = r;

		if ( max != i )
		{
			std::swap( arr[i], arr[max] );
			heapify( arr, max, size, isMaxHeap );
		}
	}
}

bool isGreater( string first, string second )
{
	if ( first.length() != second.length() )
	{
		if ( first.length() > second.length() )
			return true;
		else
			return false;
	}

	for ( int i = 0; i < first.length(); i++ )
	{
		char f = first[i], s = second[i];
		if ( f != s )
		{
			if ( f > s )
				return true;
			else
				return false;
		}
	}

	return false;
}

void buildHeap( string* arr, int size, bool isMaxHeap )
{
	for ( int i = size; i >= 0; i-- )
	{
		heapify( arr, i, size, isMaxHeap );
	}
}
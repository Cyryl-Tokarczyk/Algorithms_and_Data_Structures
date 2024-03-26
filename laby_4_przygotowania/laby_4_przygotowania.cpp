#include <iostream>

void printHeap( int* arr, int size )
{
	int j = 0;
	for ( int i = 0; i < size; i++ )
	{
		std::cout << arr[i] << ' ';

		if ( i == j )
		{
			std::cout << std::endl;
			j = j == 0 ? 2 : j + j * 2;
		}
	}
}

int parent( int i )
{
	return (i - 1) / 2;
}

int right( int i )
{
	return i * 2 + 1;
}

int left( int i )
{
	return i * 2 + 2;
}

void heapify( int* arr, int i, int size )
{
	int l = left( i );
	int r = right( i );
	int max = i;

	if ( l < size && arr[l] > arr[i] )
		max = l;

	if ( r < size && arr[r] > arr[max] )
		max = r;

	if ( max != i )
	{
		std::swap( arr[i], arr[max] );
		heapify( arr, max, size );
	}
}

void heapSort( int* arr, int size )
{
	for ( int i = size - 1; i > 0; i-- )
	{
		std::swap( arr[0], arr[i] );
		heapify( arr, 0, i );
	}
}

int main()
{
	int n;
	std::cin >> n;

	int* arr = new int[n];

	for ( int i = 0; i < n; i++ )
	{
		std::cin >> arr[i];
	}

	// Build Heap
	for ( int i = n; i > 0; i-- )
	{
		heapify( arr, i, n );
	}

	heapSort( arr, n );

	printHeap( arr, n );

	return 0;
}
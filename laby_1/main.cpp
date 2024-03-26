#include <iostream>

using namespace std;

int findSlow( int* t, int n, int toFind );
int findQuick( int* t, int n, int toFind, bool isDesc );

int main()
{
	int m;

	cin >> m;

	for ( ; m > 0; m-- )
	{
		int n;
		cin >> n;

		int* t = new int[n];
		for ( int i = 0; i < n; i++ )
			cin >> t[i];

		bool isDesc = (t[0] > t[1]);

		int p;
		cin >> p;

		int* k = new int[p];
		for ( int i = 0; i < p; i++ )
			cin >> k[i];

		for ( int i = 0; i < p; i++ )
		{
			cout << findQuick( t, n, k[i], isDesc ) << endl;
		}

		delete[] t;
		delete[] k;
	}

	return 0;
}

int findSlow( int* t, int n, int toFind )
{
	for ( int i = 0; i < n; i++ )
	{
		if ( t[i] == toFind )
			return i;
	}

	return -1;
}

int findQuick( int* t, int n, int toFind, bool isDesc )
{
	/*int pos = n / 2;

	n /= 2;

	while ( n > 0 )
	{
		n /= 2;

		if ( t[pos] >= toFind )
		{
			if ( t[pos] == toFind )
				return pos;

			pos += (isDesc ? 1 : -1) * n;
		}
		else
		{
			pos -= (isDesc ? 1 : -1) * n;
		}
	}*/

	int first = 0, last = n - 1;

	while ( n > 0 )
	{
		int mid = (last - first) / 2 + first;

		if ( t[mid] == toFind )
			return mid;

		if ( t[mid] > toFind )
		{
			if ( !isDesc )
			{
				last = mid - 1;
			}
			else
			{
				first = mid + 1;
			}
		}
		else
		{
			if ( !isDesc )
			{
				first = mid + 1;
			}
			else
			{
				last = mid - 1;
			}
		}

		n /= 2;
	}

	return -1;
}
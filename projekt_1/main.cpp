#include <iostream>
#include <cmath>

using namespace std;

int* generatePrimeNumbers( int range, int &size );
void partition( int* primes, int size, int number, int largestNumber );
void generateCombinations( int* chosen, int* primes, int index, int k, int start, int end, int number, int largestNumber );
void partitionIterative( int* primes, int size, int number, int largestNumber );
bool addsUp( int* t, int size, int n );
bool isPrime( int n );

int main()
{
	int numberOfLines;
	cin >> numberOfLines;

	for ( int i = 0; i < numberOfLines; i++ )
	{
		int number, largestNum, *primes, size = 0;
		cin >> number >> largestNum;

		if ( !isPrime( largestNum ) )
			continue;

		primes = generatePrimeNumbers( largestNum, size );

		partition( primes, size, number, largestNum );
		//partitionIterative( primes, size, number, largestNum );

		delete[] primes;
	}

	return 0;
}

int* generatePrimeNumbers( int range, int &size )
{
	int* temp = new int[range+1];

	for ( int i = 0; i < range+1; i++ )
		temp[i] = i;
	temp[1] = 0;
	
	// Sieve of Eratostenes:

	for ( int i = 2; i <= sqrt(range); i++ )
	{
		for ( int j = i*2; j < range+1; j += i )
		{
			temp[j] = 0;
		}
	}

	// Counting primes in order to know how big array is needed

	int count = 0;

	for ( int i = 0; i < range+1; i++ )
		if ( temp[i] != 0 )
			count++;
	size = count;

	// Rewriting to a new array

	int* primesArray = new int[count];

	for ( int i = 0, j = 0; i < range+1; i++ )
	{
		if ( temp[i] != 0 )
		{
			primesArray[j] = temp[i];
			j++;
		}
	}		

	delete[] temp;

	return primesArray;
}

void partition( int* primes, int size, int number, int largestNumber )
{
	int maxK = number / 2, minK = number / largestNumber;

	for ( int i = maxK - 1; i >= minK - 1; i-- )
	{
		int* chosen = new int[i];

		generateCombinations( chosen, primes, 0, i, 0, size - 1, number, largestNumber );

		delete[] chosen;
	}
}

void generateCombinations( int* chosen, int* primes, int index, int k, int start, int end, int number, int largestNumber )
{
	if ( addsUp( chosen, k, number - primes[end] ) )
	{
		cout << primes[end];
		for ( int i = k - 1; i >= 0; i-- )
		{
			cout << '+' << chosen[i];
		}
		cout << endl;
		return;
	}

	if ( index == k )
	{
		return;
	}

	if ( start > end )
		return;

	chosen[index] = primes[start];
	generateCombinations( chosen, primes, index + 1, k, start, end, number, largestNumber );
	generateCombinations( chosen, primes, index, k, start + 1, end, number, largestNumber );
}

//void partitionIterative( int* primes, int size, int number, int largestNumber )
//{
//	int maxK = number / 2, minK = number / largestNumber;
//
//	for ( int i = maxK - 1; i >= minK - 1; i-- )
//	{
//		int* chosen = new int[i];
//
//		for ( int start = 0; start < size; start++ )
//		{
//			for ( int index = 0; index <= i; index++ )
//			{
//				if ( addsUp( chosen, i, number - primes[size - 1] ) )
//				{
//					cout << primes[size - 1];
//					for ( int l = i - 1; l >= 0; l-- )
//					{
//						cout << '+' << chosen[l];
//					}
//					cout << endl;
//					break;
//				}
//
//				chosen[index] = primes[start];
//			}
//		}
//
//		delete[] chosen;
//	}
//}

bool addsUp( int* t, int size, int n )
{
	int sum = 0;

	for ( int i = 0; i < size; i++ )
	{
		sum += t[i];
	}

	return sum == n ? true : false;
}

bool isPrime( int n )
{
	for ( int i = 2; i <= sqrt(n); i++ )
	{
		if ( n % i == 0 )
			return false;
	}

	return true;
}
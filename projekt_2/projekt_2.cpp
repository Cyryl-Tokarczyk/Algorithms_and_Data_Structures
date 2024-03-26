//#define _CRTDBG_MAP_ALLOC
#include <minmax.h>
//#include <crtdbg.h>

#include "block.h"

using namespace std;

/*
	Struktura bloku:
	4 B - wskaznik na nastepny
	4 B - wskaznik na poprzedni
	4 B - liczba elementow w tym bloku
	4 B - wypelnienie, zeby offset byl podzielny przez 8
	8 B x n - elementy

	Iterator - zwykly wskaznik na element ( miejsce w pamieci po ktorym jest element zapisany.
	Przy kazdym ruszaniu elementow funkcja sprawdzajaca czy nie zmienia to czegos dla iteratorow. ( poczatek bloku <= iterator && iterator <= koniec bloku ).
	^ bez sensu i tak zeby poznac kontekst trzeba bedzie przeiterowac po wszystkich blokach
	Lepiej pamietac po prostu nr elementu, duzo prostsze w implementacji i zarzadzaniu.
*/

constexpr int numOfIterators = 10;
constexpr int MaxNumberLength = 20;

void checkForOptimalAllocation( uint8_t*& head, uint8_t*& tail, int blockSize );
void incrementIterators( int* t, int after );
void decrementIterators( int* t, int after );
void addBehind( uint8_t*& head, uint8_t*& tail, int blockSize, uint8_t* where, uint64_t value );
void addAfter( uint8_t*& head, uint8_t*& tail, int blockSize, uint8_t* where, uint64_t value );
void removeElement( uint8_t*& head, uint8_t*& tail, int blockSize, uint8_t* where );
void printAll( uint8_t* head, int blockSize );
void deleteList( uint8_t* head );

int main()
{
	//_CrtSetDbgFlag( _CRTDBG_CHECK_ALWAYS_DF );

	char command[10];
	int iterators[numOfIterators];
	for (int i = 0; i < numOfIterators; i++)
		iterators[i] = -1;

	uint8_t* head = nullptr, * tail = nullptr;

	cin >> command;

	int blockSize;
	cin >> blockSize;

	while (cin >> command)
	{
		int totalNumberOfElements = getTotalNumberOfElements( head );
		if (!strcmp( command, "i" ))
		{
			int iterator;
			char position[MaxNumberLength];

			cin >> iterator >> position;

			if (!strcmp( position, "BEG" ))
			{
				iterators[iterator] = 0;
			}
			else if (!strcmp( position, "END" ))
			{
				iterators[iterator] = totalNumberOfElements - 1;
			}
			else
			{
				int pos = strtol( position, nullptr, 0 );
				iterators[iterator] = iterators[pos];
			}
		}
		else if (!strcmp( command, "+" ))
		{
			int iterator;
			cin >> iterator;
			if( iterators[iterator] < totalNumberOfElements - 1 )
				iterators[iterator]++;
		}
		else if (!strcmp( command, "-" ))
		{
			int iterator;
			cin >> iterator;
			if (iterators[iterator] > 0)
				iterators[iterator]--;
		}
		else if (!strcmp( command, ".A" ))
		{
			uint64_t value;
			char iterator[MaxNumberLength];
			cin >> iterator >> value;

			if (!strcmp( iterator, "BEG" ))
			{
				addBehind( head, tail, blockSize, head + offset, value );
				incrementIterators( iterators, -1 );
				totalNumberOfElements++;
			}
			else if (!strcmp( iterator, "END" ))
			{
				addBehind( head, tail, blockSize, getLastElementPointer( tail ), value );
				incrementIterators( iterators, totalNumberOfElements - 2 );
				totalNumberOfElements++;
			}
			else
			{
				int i = strtol( iterator, nullptr, 0 );
				addBehind( head, tail, blockSize, getElementPointer( head, iterators[i] ), value );
				incrementIterators( iterators, iterators[i] - 1 );
				totalNumberOfElements++;
			}
		}
		else if (!strcmp( command, "A." ))
		{
			uint64_t value;
			char iterator[MaxNumberLength];
			cin >> iterator >> value;

			if (!strcmp( iterator, "BEG" ))
			{
				addAfter( head, tail, blockSize, head + offset - elementSize, value );
				incrementIterators( iterators, 0 );
				totalNumberOfElements++;
			}
			else if (!strcmp( iterator, "END" ))
			{
				addAfter( head, tail, blockSize, getLastElementPointer( tail ), value );
				totalNumberOfElements++;
				// No incrementing necessary
			}
			else
			{
				int i = strtol( iterator, nullptr, 0 );
				addAfter( head, tail, blockSize, getElementPointer( head, iterators[i] ), value );
				incrementIterators( iterators, iterators[i] );
				totalNumberOfElements++;
			}
		}
		else if (!strcmp( command, "R" ))
		{
			char iterator[MaxNumberLength];
			cin >> iterator;
			if (!strcmp( iterator, "BEG" ))
			{
				removeElement( head, tail, blockSize, head + offset );
				decrementIterators( iterators, 0 );
				totalNumberOfElements--;
			}
			else if (!strcmp( iterator, "END" ))
			{
				removeElement( head, tail, blockSize, getLastElementPointer( tail ) );
				decrementIterators( iterators, totalNumberOfElements - 2 );
				totalNumberOfElements--;
			}
			else
			{
				int i = strtol( iterator, nullptr, 0 );
				removeElement( head, tail, blockSize, getElementPointer( head, iterators[i] ) );
				decrementIterators( iterators, min(iterators[i], totalNumberOfElements - 2) );
				totalNumberOfElements--;
			}
		}
		else if (!strcmp( command, "P" ))
		{
			char iterator[MaxNumberLength];
			cin >> iterator;
			if (!strcmp( iterator, "ALL" ))
			{
				printAll( head, blockSize );
			}
			else
			{
				int i = strtol( iterator, nullptr, 0 );
				uint64_t val;
				memcpy( &val, getElementPointer( head, iterators[i] ), elementSize );
				cout << val << endl;
			}
		}
		else if (!strcmp( command, "Q" ))
		{
			break;
		}
	}

	deleteList( head );

	//_CrtDumpMemoryLeaks();

	return 0;
}

void checkForOptimalAllocation( uint8_t*& head, uint8_t*& tail, int blockSize )
{
	int totalNumOfElements = getNumberOfElements( head ), maxDiff = 0,
		previousNum = getNumberOfElements( head ), numOfBlocks = 1,
		maxNumPerBlock = (blockSize - offset) / elementSize;
	uint8_t* current = getNext( head );

	while (current)
	{
		int numOfEl = getNumberOfElements( current );
		totalNumOfElements += numOfEl;
		if (abs( numOfEl - previousNum ) > maxDiff)
			maxDiff = abs( numOfEl - previousNum );

		numOfBlocks++;
		current = getNext( current );
	}

	bool tooManyBlocks = ((numOfBlocks > 1) && (totalNumOfElements / (numOfBlocks - 1) < maxNumPerBlock));

	if (maxDiff > 1 || tooManyBlocks)
	{
		int numOfElementsPerBlock, remainder;
		if (tooManyBlocks)
		{
			numOfElementsPerBlock = totalNumOfElements / (numOfBlocks - 1);
			remainder = totalNumOfElements % (numOfBlocks - 1);
		}
		else
		{
			numOfElementsPerBlock = totalNumOfElements / numOfBlocks;
			remainder = totalNumOfElements % numOfBlocks;
		}
		current = head;

		uint8_t* temp = nullptr;
		uint32_t tempSize = 0;

		while (current)
		{
			int numOfEl = getNumberOfElements( current );
			uint8_t* next = getNext( current );

			if (tooManyBlocks && next == nullptr)
			{
				tail = getPrev( current );
				deleteBlock( current );
				break;
			}

			if (tempSize > 0)
			{
				int desiredNumOfElements = (numOfElementsPerBlock + (remainder > 0 ? 1 : 0));
				int howMuchLeft = ((tempSize / elementSize) + numOfEl) - desiredNumOfElements;

				if (howMuchLeft <= 0)
				{
					memcpy( current + offset + tempSize, current + offset, numOfEl * elementSize ); // Move forward
					memcpy( current + offset, temp, tempSize ); // Copy from temp

					numOfEl += (tempSize / elementSize); // Update number of elements
					setNumberOfElements( current, numOfEl );

					tempSize = 0; // Deallocate temp
					temp = (uint8_t*)realloc( temp, tempSize );

				}
				else if (tempSize / elementSize > desiredNumOfElements)
				{
					uint32_t fromTemp = tempSize / elementSize - desiredNumOfElements;

					uint8_t* t = (uint8_t*)malloc( fromTemp * elementSize + howMuchLeft * elementSize );

					memcpy( t, temp + tempSize - fromTemp * elementSize, fromTemp * elementSize ); // Copying leftovers from temp
					memcpy( t + fromTemp * elementSize, current + offset, numOfEl * elementSize ); // Copying all elements from the block

					memcpy( current + offset, temp, desiredNumOfElements * elementSize ); // Copying from temp to the block
					numOfEl = desiredNumOfElements; // Updating number of elements
					setNumberOfElements( current, numOfEl );

					tempSize = fromTemp * elementSize + howMuchLeft * elementSize; // Reallocate temp to store data that was left
					temp = (uint8_t*)realloc( temp, tempSize );

					memcpy( temp, t, tempSize );

					free( t );
				}
				else
				{
					uint8_t* t = (uint8_t*)malloc( howMuchLeft * elementSize );

					memcpy( t, current + offset + (numOfEl - howMuchLeft) * elementSize, howMuchLeft * elementSize ); // Copy elements that are left to a new block

					numOfEl -= howMuchLeft;

					memcpy( current + offset + tempSize, current + offset, numOfEl * elementSize ); // Move forward
					memcpy( current + offset, temp, tempSize ); // Copy from temp

					numOfEl += (tempSize / elementSize); // Update number of elements
					setNumberOfElements( current, numOfEl );

					tempSize = howMuchLeft * elementSize; // Reallocate temp to store data that was left
					temp = (uint8_t*)realloc( temp, tempSize );

					memcpy( temp, t, tempSize );

					free( t );
				}

			}
			if (numOfEl < numOfElementsPerBlock + (remainder > 0 ? 1 : 0) && next)
			{
				int howMuch = numOfElementsPerBlock + (remainder > 0 ? 1 : 0) - numOfEl;
				int numOfElInNext = getNumberOfElements( next );

				if (numOfEl == 0)
				{
					deleteBlock( current );
					createNewBlockAfter( tail, blockSize, tail );
					current = next;
					continue;
				}
				if (howMuch > numOfElInNext)
				{
					// Copy from next
					memcpy( current + offset + elementSize * numOfEl, next + offset, numOfElInNext * elementSize );
					// update num of el
					numOfEl += numOfElInNext;
					setNumberOfElements( current, numOfEl );
					// delete next
					deleteBlock( next );
					// set new next
					next = getNext( current );

					howMuch = numOfElementsPerBlock + (remainder > 0 ? 1 : 0) - numOfEl;
				}

				int newNumOfElInNext = getNumberOfElements( next ) - howMuch;

				// Get some from the next one

				memcpy( current + offset + elementSize * numOfEl, next + offset, howMuch * elementSize ); // Copy from next
				memcpy( next + offset, next + offset + howMuch * elementSize, newNumOfElInNext * elementSize ); // Move elements in the next block forward

				uint32_t newNumOfEl = howMuch + numOfEl; // Update number of elements in the current block
				setNumberOfElements( current, newNumOfEl );

				setNumberOfElements( next, newNumOfElInNext ); // Update number of elements in the next block

				if (remainder != 0)
					remainder--;
			}
			else if (numOfEl > numOfElementsPerBlock + (remainder > 0 ? 1 : 0) && next)
			{
				// Store leftover elements in temp

				int howMuch = numOfEl - (numOfElementsPerBlock + (remainder > 0 ? 1 : 0));
				tempSize = howMuch * elementSize;
				if (temp == nullptr)
					temp = (uint8_t*)malloc( tempSize );
				else
					temp = (uint8_t*)realloc( temp, tempSize );

				memcpy( temp, current + offset + (numOfEl - howMuch) * elementSize, tempSize ); // Copy leftover elements to temp

				numOfEl -= howMuch; // Update number of elements
				setNumberOfElements( current, numOfEl );

				if (remainder != 0)
					remainder--;
			}
			else // Otherwise it's exactly how it should be
			{
				if (remainder != 0)
					remainder--;
			}

			current = next;
		}

		if (tempSize != 0)
		{
			cout << "W tempie cos zostalo!";
			exit( 1 );
		}
	}

	//_CrtCheckMemory();
}

void incrementIterators( int* t, int after )
{
	for (int i = 0; i < numOfIterators; i++)
	{
		if ( t[i] > after )
		{
			t[i]++;
		}
	}
}

void decrementIterators( int* t, int after )
{
	for (int i = 0; i < numOfIterators; i++)
	{
		if (t[i] > after)
		{
			t[i]--;
		}
	}
}

void addBehind( uint8_t*& head, uint8_t*& tail, int blockSize, uint8_t* where, uint64_t value )
{
	// If list isn't initialized
	if (head == nullptr)
	{
		createNewBlock( head, blockSize );
		tail = head;
		where = head + offset;
	}

	// Check what block
	uint8_t* block = findBlock( head, where, blockSize );

	// Check if block not full, if full just create a new one, elemnts will be reallocated in the last step
	if (isBlockFull( block, blockSize ))
	{
		createNewBlock( head, blockSize, block );
		uint8_t* next = block;
		block = getPrev( block );

		memcpy( block + offset, next + offset, where - (next + offset) ); // Copying elements to the new block
		memcpy( next + offset, where, blockSize - (where - next) ); // Moving elements back

		uint32_t numOfElementsMoved = (where - (next + offset)) / elementSize;
		setNumberOfElements( block, numOfElementsMoved );
		setNumberOfElements( next, getNumberOfElements( next ) - numOfElementsMoved );

		where = getLastElementPointer( block ) + elementSize;
	}

	memcpy( where + elementSize, where, blockSize - (where + elementSize - block) ); // Moving elements forward to make space
	memcpy( where, &value, elementSize ); // Placing element

	// Incrementing number of elements in the block
	incrementNumberOfElements( block );

	// Check if there isn't a need to reallocate elements in blocks
	checkForOptimalAllocation( head, tail, blockSize );
}

void addAfter( uint8_t*& head, uint8_t*& tail, int blockSize, uint8_t* where, uint64_t value )
{
	// If list isn't initialized
	if (head == nullptr)
	{
		createNewBlock( head, blockSize );
		tail = head;
		where = head + offset - elementSize;
	}

	// Check what block
	uint8_t* block = findBlock( head, where, blockSize );

	// Check if block not full, if full just create a new one, elemnts will be reallocated in the last step
	if (isBlockFull( block, blockSize )) //! Works only for the last element in the block!
	{
		createNewBlockAfter( tail, blockSize, block );
		uint8_t* next = getNext( block );

		memcpy( next + offset, where + elementSize, blockSize - (where + elementSize - block) ); // Copying to the new block that was created in front

		uint32_t numOfElementsMoved = (blockSize - (where + elementSize - block)) / elementSize;
		setNumberOfElements( block, getNumberOfElements( block ) - numOfElementsMoved );
		setNumberOfElements( next, numOfElementsMoved );

		where = next + offset - elementSize;
		block = next;
	}

	memcpy( where + 2 * elementSize, where + elementSize, blockSize - (where + 2 * elementSize - block) ); // Moving elements forward to make space. Na pewno *2 przy elSize?
	memcpy( where + elementSize, &value, elementSize ); // Placing element

	// Incrementing number of elements in the block
	incrementNumberOfElements( block );

	// Check if there isn't a need to reallocate elements in blocks
	checkForOptimalAllocation( head, tail, blockSize );
}

void removeElement( uint8_t*& head, uint8_t*& tail, int blockSize, uint8_t* where )
{
	// Check what block
	uint8_t* block = findBlock( head, where, blockSize );

	memcpy( where, where + elementSize, blockSize - (where + elementSize - block) ); // Moving elements back

	decrementNumberOfElements( block );

	checkForOptimalAllocation( head, tail, blockSize );
}

void printAll( uint8_t* head, int blockSize )
{
	uint8_t* current = head;
	int i = 0;

	while (current)
	{
		//cout << "Block " << i << ":" << endl;
		for (int i = 0; i < getNumberOfElements( current ); i++)
		{
			cout << getElement( current, i ) << ' ';
		}
		current = getNext( current );
		//cout << endl;
		i++;
	}
	cout << endl;
}

void deleteList( uint8_t* head )
{
	uint8_t* current = head, * prev = nullptr;

	while (current)
	{
		prev = current;
		current = getNext( current );

		delete prev;
	}
}
#include "block.h"

uint8_t* initBlock( int size )
{
	uint8_t* block = (uint8_t*)malloc( size );
	memset( block, 0, size );

	return block;
}

void deleteBlock( uint8_t* block )
{
	uint8_t* prev = getPrev( block ), * next = getNext( block );
	if (prev)
		setNext( prev, next );
	if (next)
		setPrev( next, prev );

	free( block );
}

void createNewBlock( uint8_t*& head, int blockSize/*, int &numberOfBlocks*/ )
{
	/* numberOfBlocks++; */
	uint8_t* newBlock = initBlock( blockSize );
	head = newBlock;
}

void createNewBlock( uint8_t*& head, int blockSize/*, int &numberOfBlocks*/, uint8_t* blockInFront )
{
	/* numberOfBlocks++; */
	uint8_t* newBlock = initBlock( blockSize );
	uint8_t* prev = nullptr;
	uint8_t* next = blockInFront;
	if (next)
		prev = getPrev( next );

	if (prev)
		setNext( prev, newBlock );
	if (next)
		setPrev( next, newBlock );

	setNext( newBlock, next );
	setPrev( newBlock, prev );

	if (head == nullptr)
		head = newBlock;

	if (head == blockInFront)
		head = newBlock;
}

void createNewBlockAfter( uint8_t*& tail, int blockSize/*, int &numberOfBlocks*/, uint8_t* blockBehind )
{
	/* numberOfBlocks++; */
	uint8_t* newBlock = initBlock( blockSize );
	uint8_t* prev = blockBehind;
	uint8_t* next = nullptr;
	if (prev)
		next = getNext( prev );

	if (prev)
		setNext( prev, newBlock );
	if (next)
		setPrev( next, newBlock );

	setNext( newBlock, next );
	setPrev( newBlock, prev );

	if (tail == nullptr)
		tail = newBlock;

	if (tail == blockBehind)
		tail = newBlock;
}

uint8_t* findBlock( uint8_t* head, int numberOfBlock )
{
	uint8_t* result = head;

	for (int i = 0; i < numberOfBlock; i++)
	{
		result = getNext( result );
	}

	if (result == nullptr)
	{
		std::cout << "HALKO";
		exit( 1 );
	}

	return result;
}

uint8_t* findBlock( uint8_t* head, uint8_t* pointer, int blockSize )
{
	uint8_t* result = head;

	while (!(result <= pointer && pointer <= result + blockSize))
	{
		result = getNext( result );
	}

	if (result == nullptr)
	{
		std::cout << "HALKO";
		exit( 1 );
	}

	return result;
}

uint8_t* getNext( uint8_t* block )
{
	uint8_t* next;
	memcpy( &next, block, pointerSize );
	return next;
}

uint8_t* getPrev( uint8_t* block )
{
	uint8_t* prev;
	memcpy( &prev, block + prevOffset, pointerSize );
	return prev;
}

uint32_t getNumberOfElements( uint8_t* block )
{
	return *((uint32_t*)(block + numOffset));
}

int getTotalNumberOfElements( uint8_t* head )
{
	int result = 0;
	uint8_t* current = head;

	while (current)
	{
		result += getNumberOfElements( current );
		current = getNext( current );
	}

	return result;
}

uint64_t getElement( uint8_t* block, int number )
{
	return *((uint64_t*)(block + offset + elementSize * number));
}

uint8_t* getElementPointer( uint8_t* head, int number )
{
	int sum = 0;
	uint8_t* current = head;

	while (current)
	{
		int currentNumOfEl = getNumberOfElements( current );

		if (sum + currentNumOfEl > number)
		{
			return current + offset + (number - sum) * elementSize;
		}

		sum += currentNumOfEl;
		current = getNext( current );
	}

	return nullptr;
}

uint8_t* getLastElementPointer( uint8_t* block )
{
	int numOfEl;
	if (block)
		numOfEl = getNumberOfElements( block );
	else
		numOfEl = 0;

	return block + offset + (numOfEl - 1) * elementSize;
}

void setNext( uint8_t* block, uint8_t* next )
{
	memcpy( block, &next, pointerSize );
}

void setPrev( uint8_t* block, uint8_t* prev )
{
	memcpy( block + prevOffset, &prev, pointerSize );
}

bool isBlockFull( uint8_t* block, int blockSize )
{
	return getNumberOfElements( block ) >= (blockSize - offset) / elementSize ? true : false;
}

void incrementNumberOfElements( uint8_t* block )
{
	uint32_t numberOfEl = getNumberOfElements( block ) + 1;
	memcpy( block + numOffset, &numberOfEl, intSize );
}

void decrementNumberOfElements( uint8_t* block )
{
	uint32_t numberOfEl = getNumberOfElements( block ) - 1;
	memcpy( block + numOffset, &numberOfEl, intSize );
}

void setNumberOfElements( uint8_t* block, uint32_t num )
{
	memcpy( block + numOffset, &num, intSize );
}
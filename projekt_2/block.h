#pragma once

//#define _CRTDBG_MAP_ALLOC

#include <iostream>
//#include <crtdbg.h>

const int pointerSize = sizeof( uint8_t* );
const int elementSize = sizeof( uint64_t );
const int intSize = sizeof( uint32_t );
const int offset = 2 * pointerSize + 2 * intSize;
constexpr int prevOffset = pointerSize;
constexpr int numOffset = 2 * pointerSize;

uint8_t* initBlock( int size );
void deleteBlock( uint8_t* block );
void createNewBlock( uint8_t*& head, int blockSize/*, int &numberOfBlocks*/ );
void createNewBlock( uint8_t*& head, int blockSize/*, int &numberOfBlocks*/, uint8_t* blockInFront );
void createNewBlockAfter( uint8_t*& tail, int blockSize/*, int &numberOfBlocks*/, uint8_t* blockBehind );
uint8_t* findBlock( uint8_t* head, int numberOfBlock );
uint8_t* findBlock( uint8_t* head, uint8_t* pointer, int blockSize );
uint8_t* getNext( uint8_t* block );
uint8_t* getPrev( uint8_t* block );
uint32_t getNumberOfElements( uint8_t* block );
int getTotalNumberOfElements( uint8_t* head );
uint64_t getElement( uint8_t* block, int number );
uint8_t* getElementPointer( uint8_t* head, int number );
uint8_t* getLastElementPointer( uint8_t* block );
void setNext( uint8_t* block, uint8_t* next );
void setPrev( uint8_t* block, uint8_t* prev );
bool isBlockFull( uint8_t* block, int blockSize );
void incrementNumberOfElements( uint8_t* block );
void decrementNumberOfElements( uint8_t* block );
void setNumberOfElements( uint8_t* block, uint32_t num );
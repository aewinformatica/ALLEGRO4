//-----------------------------------------------------------------------------------------
//---------- made by Peter Jamróz 2001 ----------------------------------------------------
//-----------------------------------------------------------------------------------------
//This file implements improved Queue class which will be used by MyServSockets class
//and GRPC server and client
//this is probably optimal block size
#define QUEUE_BLOCK_SIZE	8192

//#define QUEUE_BLOCK_SIZE 50

//for test and debugging

//#define QUEUE_BLOCK_SIZE 10000000

//For debugging purposes

//#define QUEUE_BLOCK_SIZE 3

//-------------------------------------------------------------------------------------------

// This class is used internally by Queue class. Please don't use it

// This is queue block. It behaves like queue, but it's size is limited.

// It can point to another block,and we can have a list of blocks.

// Queue is a list of blocks
class	qblock
{
		//this points to first byte of data in this block
		int		BeginPointer;

		//this points to last byte of data in this block
		int		EndPointer;

		char	buffer[QUEUE_BLOCK_SIZE];
	public:
		qblock();
		~		qblock();

		//this variable points to previous block or NULL if it is end of the queue
		qblock	*prev;

		int		IsFirstBlock();

		//int IsLastBlock();
		//it returns true if block is full
		int		IsFull();

		//this function reads up to RequestedLength bytes to block. Function returns number of bytes succesfully read.
		int		Read(void *output, int RequestedLength);

		//this function peeks data and it doesn't remove it from block
		int		PeekData(void *output, int RequestedLength);

		//this function returns number of bytes stored in this block
		int		GetLength();

		//this function writes RequestedLength bytes to block.Function returns number of bytes succesfully written.
		int		Write(void *input, int RequestedLength);
};

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
#ifndef __QUEUE__
#define __QUEUE__

class	Queue
{
	private:
		qblock	*FirstBlock;
		qblock	*LastBlock;

		//these are internal function to expand and shrink the queue, and user programmen
		//should not use them.
		int		Expand();
		int		RemoveLastBlock();
	public:
		//it creates new, empty queue
		Queue();

		//it destroys queue and all blocks which it contains
		~Queue();

		//this function reads up to RequestedLength bytes to buffer. Function returns number of bytes succesfully read.
		int Read(void *output, int RequestedLength);

		//This function behaves like read, except that it does not remove data from the queue
		int PeekData(void *output, int RequestedLength);

		//this function returns number of bytes stored in this queue
		int GetLength();

		//this function writes up to RequestedLength bytes to queue.Function returns number of bytes succesfully written.
		//function will return value less than RequestedLength only in case when there are be memory allocation problems.
		int Write(void *input, int RequestedLength);
};
#endif

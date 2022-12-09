#include "MemoryManager.h"

#include <iomanip>
#include <iostream>
using namespace std;

namespace MemoryManager
{
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT 
	//
	// This is the only static memory that you may use, no other global variables may be
	// created, if you need to save data make it fit in MM_pool
	//
	// IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT


	const int MM_POOL_SIZE = 65536;
	char MM_pool[MM_POOL_SIZE];

	// I have provided this tool for your use
	void memView(int start, int end)
	{

		const unsigned int SHIFT = 8;
		const unsigned int MASK = 1 << SHIFT - 1;

		unsigned int value;	// used to facilitate bit shifting and masking

		cout << "               Pool                     Unsignd  Unsigned " << endl;
		cout << "Mem Add        indx   bits   chr ASCII#  short      int    " << endl;
		cout << "-------------- ---- -------- --- ------ ------- ------------" << endl;

		for (int i = start; i <= end; i++)
		{
			cout << (long*)(MM_pool + i) << ':';	// the actual address in hexadecimal
			cout << '[' << setw(2) << i << ']';				// the index into MM_pool

			value = MM_pool[i];
			cout << " ";
			for (int j = 1; j <= SHIFT; j++)		// the bit sequence for this byte (8 bits)
			{
				cout << ((value & MASK) ? '1' : '0');
				value <<= 1;
			}
			cout << " ";

			cout << '|' << *(char*)(MM_pool + i) << "| (";		// the ASCII character of the 8 bits (1 byte)
			cout << setw(4) << ((int)(*((unsigned char*)(MM_pool + i)))) << ")";	// the ASCII number of the character

			cout << " (" << setw(5) << (*(unsigned short*)(MM_pool + i)) << ")";	// the unsigned short value of 16 bits (2 bytes)
			cout << " (" << setw(10) << (*(unsigned int*)(MM_pool + i)) << ")";	// the unsigned int value of 32 bits (4 bytes)

																				//  cout << (*(unsigned int*)(MM_pool+i)) << "|";	// the unsigned int value of 32 bits (4 bytes)

			cout << endl;
		}
	}

	// Initialize set up any data needed to manage the memory pool
	void initializeMemoryManager(void)
	{

		int freeHead = 0; // starting index of the freelist
		int inUseHead = 2; // starting index of the inUselist
		int usedHead = 4; // starting index for the used list - deallocated memory

		int nextLink = 2; // offset index of the next link
		int prevLink = 4; // offset index for the prev link

		*(unsigned short*)(MM_pool + freeHead) = 6; // freelist starts at byte 6
		*(unsigned short*)(MM_pool + 6) = MM_POOL_SIZE - 6; // we used 6 bytes to get things started
		*(unsigned short*)(MM_pool + inUseHead) = 0;	// nothing in the inUse list yet
		*(unsigned short*)(MM_pool + usedHead) = 0; // nothing in the used list yet

	}

	// return a pointer inside the memory pool
	// If no chunk can accommodate aSize call onOutOfMemory() - still 
	void* allocate(int aSize)
	{
		if ((int)(*(unsigned short*)(MM_pool)) + aSize + 6 > 65536)
			onOutOfMemory();
	// TBD

		int freeHead = 0; // starting index of the freelist
		int inUseHead = 2; // starting index of the inUselist
		int usedHead = 4; // starting index for the used list - deallocated memory

		int trueSize = aSize + 6; // give each new node 6 bytes to store size, next, and previous
		int freeMemIndex = *(unsigned short*)(MM_pool + freeHead); // Index of start of free memory list
		int inUseMemIndex = *(unsigned short*)(MM_pool + inUseHead); //Index of start of in use memory
		int newFreeMemIndex = *(unsigned short*)(MM_pool + freeHead) + trueSize; // Index of start of new free memory after insertion

		int totalFreeMem = MM_POOL_SIZE - newFreeMemIndex;

		int prevLink = inUseMemIndex + 4;

		int newNextLink = freeMemIndex + 2;
		int newPrevLink = freeMemIndex + 4;

		int newInUseMemIndex = *(unsigned short*)(MM_pool + inUseHead);

		*(unsigned short*)(MM_pool + inUseHead) = freeMemIndex;
		*(unsigned short*)(MM_pool + freeHead) = newFreeMemIndex;

		if(freeMemIndex != 6)
			*(unsigned short*)(MM_pool + prevLink) = freeMemIndex;

		*(unsigned short*)(MM_pool + newNextLink) = inUseMemIndex;
		*(unsigned short*)(MM_pool + newPrevLink) = 0;

		*(unsigned short*)(MM_pool + freeMemIndex) = aSize;

		*(unsigned short*)(MM_pool + newFreeMemIndex) = totalFreeMem;

		return (void*)(MM_pool + *(unsigned short*)MM_pool - aSize);

	}

	// Free up a chunk previously allocated
	void deallocate(void* aPointer)
	{
	// TBD

		int freeHead = 0; // starting index of the freelist
		int inUseHead = 2; // starting index of the inUselist
		int usedHead = 4; // starting index for the used list - deallocated memory

		int deleteNodeIndex = (char*)aPointer - MM_pool - 6;

		int prevNodeIndex = *(unsigned short*)(MM_pool + deleteNodeIndex + 2);
		int nextNodeIndex = *(unsigned short*)(MM_pool + deleteNodeIndex + 4);

		*(unsigned short*)(MM_pool + prevNodeIndex + 4) = nextNodeIndex;
		*(unsigned short*)(MM_pool + nextNodeIndex + 2) = prevNodeIndex;

		int usedMemIndex = *(unsigned short*)(MM_pool + usedHead);

		*(unsigned short*)(MM_pool + deleteNodeIndex + 4) = 0;
		*(unsigned short*)(MM_pool + deleteNodeIndex + 2) = usedMemIndex;

		*(unsigned short*)(MM_pool + usedMemIndex + 2) = deleteNodeIndex;

		*(unsigned short*)(MM_pool + usedHead) = deleteNodeIndex;
		

	}
	// returns the number of bytes allocated by ptr
	int size(void *ptr)
	{
	// TBD
		return sizeof(ptr);
		
	}
	
	// Will scan the memory pool and return the total free space remaining
	int freeMemory(void)
	{
	// TBD
		int freeHead = 0; // starting index of the freelist
		int freeMem = MM_POOL_SIZE - *(unsigned short*)(MM_pool + freeHead);
		return freeMem;
	}

	// Will scan the memory pool and return the total used memory - memory that has been deallocated
	int usedMemory(void)
	{
	// TBD
		int usedMem = 0;
		int nextNode = *(unsigned short*)(MM_pool + 4);

		while (nextNode > 0)
		{
			usedMem = usedMem + *(unsigned short*)(MM_pool + nextNode) + 6;
			nextNode = *(unsigned short*)(MM_pool + nextNode + 2);
		}

		return usedMem;
	}

	// Will scan the memory pool and return the total in use memory - memory being curretnly used
	int inUseMemory(void)
	{
	// TBD
		int inUseMem = MM_POOL_SIZE - freeMemory() - usedMemory() - 6;
		return inUseMem;
	}

	void onOutOfMemory(void)
 	{
    std::cout << "\nMemory pool out of memory\n" << std::endl;
    std::cout << "\n---Press \"Enter\" key to end program---:";
	cin.get();
    exit( 1 );  // exit main
  }
}

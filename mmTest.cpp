/*  This is the driver for your memory manager. Do not make any changes to this file
    Compile it with your MemeryManager.h and MemoryManager.cpp files.
    The output that I got is at the bottom. Your ourput should be similar.
    Notice that Total Memory = Free Memory + InUse Memory + Used Memory

    Include this file when you submit your program
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <climits>

#include "MemoryManager.h"
using namespace std;
using namespace MemoryManager;  // allow Memory Manager functions to be used without prefix

void memStats();

int main(void)
{

    initializeMemoryManager();
    cout << "\nmemView immediately after initializeMemoryManager:\n";
    memView(0,10);
        
    int alloc = 0;
    int dealloc = 0;
    int start = freeMemory();

    cout<<"Program Starting...";
    memStats();

    cout<<"\nAllocate some memory:";
    
    short* shortPtr = (short*) allocate(sizeof(short));
    char* charPtr = (char*)allocate(sizeof(char));
    int* intPtr = (int*)allocate(sizeof(int));
    char* lname = (char*)allocate(11);
    char *fname = (char*)allocate(10);
    int * maxIntPtr = (int*) allocate(sizeof(int));

    memStats();
    
    cout << "\nAssign some values:";

    *maxIntPtr = INT_MAX;
    strcpy(fname, "Steve");
    strcpy(lname, "Gold");
    *shortPtr = 712;
    *charPtr = 'Z';
    *intPtr = 1000000;
    memView(0,80);

    memStats();

    cout << "\nFree Memory Avaialable before *DEATH:"<<(unsigned short)freeMemory()<<endl;
    char *death = (char*)allocate(freeMemory()-10);

    cout << "\nFree Memory Avaialable after *DEATH:"<<(unsigned short)freeMemory()<<endl;

    cout << "Size of lname: "<<size(lname)<<endl;
    cout << "Size of Death Star: "<<(unsigned short)size(death)<<endl;

    deallocate (death);
    
    cout << "\nFree Memory Avaialable after deallocating *DEATH:"<<(unsigned short)freeMemory()<<endl;

    memStats();

    cout << "\nForcing out-of-memory condition:";
    int *zPtr = (int *) allocate(4);
    cout << "\nYou will never see this line";
    return 0;
}

 
void memStats()
{
    int total = freeMemory()+inUseMemory()+usedMemory();
    cout << "\n#######################################";
    cout << "\n#Total:"<<total<<" Free:"<<freeMemory()<<" InUse:"<<inUseMemory()<<" Used:"<<usedMemory();
    cout << "\n#Press return to continue...";
    cout << "\n#######################################";
    cin.get();
}

// output from this program:
/*


*/



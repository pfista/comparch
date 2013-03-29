/* UTEID: mp25234 program 3 */
#include <stdio.h>
#include <stdlib.h>

#define MEMORY 131072 // virtual memory size in bytes, size is equal to (16^4)*2
#define MAGIC_NUM 0x7962 // y86 signifier

typedef enum { false, true } bool;

/******** FUNCTIONS *********/

/*
 * Checks to see if the data is valid for Y86 while loading the data into
 * virtual memory
 */
int loadAndValidate(unsigned char* buffer, unsigned long length);

/*
 * pre: memory has been allocated
 * load bytes into memory consecutively
 */
void loadIntoMemory (unsigned char* buffer, unsigned short byte_count,
                        unsigned short loadAddress);

void initMemory(unsigned int size);

/*
 * Prints all memory between the mix and max load address
 * in little endian with corresponding y86 instructions
 */
void printMemory(void);

/* 
 * Read an address from memory.
 */
unsigned int readAddress(unsigned short loadAddress);

char* getOpcode(int opcode);

/******** END FUNCTIONS *********/

/******* VARIABLES *********/

static unsigned char* memPtr; //TODO hide in another file? like memory.c
static bool memoryHasInitialized;
unsigned int maxAddress = 0;
unsigned int minAddress = MEMORY;

/******** END VARIABLES *********/

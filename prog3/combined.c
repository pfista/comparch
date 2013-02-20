/* UTEID: mp25234 program 2 */
#include <stdio.h>
#include <stdlib.h>

#define MEMORY 131072 // virtual memory size in bytes, size is equal to (16^4)*2
#define MAGIC_NUM 0x7962 // y86 signifier

typedef enum { false, true } bool;

int loadAndValidate(unsigned char* buffer, unsigned long length);
void loadIntoMemory (unsigned char* buffer, unsigned short byte_count,
                        unsigned short loadAddress);
void initMemory(unsigned int size);
void printMemory(void);
unsigned int readAddress(unsigned short loadAddress);
char* getOpcode(int opcode);

struct Block{ //TODO
    unsigned short load_address;
    unsigned short byte_count;
    unsigned char* data;
} Block;

static unsigned char* memPtr; //TODO this should probably be hidden in another file
static bool memoryHasInitialized;
unsigned int maxAddress = 0;
unsigned int minAddress = MEMORY;

/* END HEADER FILE */
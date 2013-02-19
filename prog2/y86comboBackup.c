/* UTEID: mp25234 program 2 */
#include <stdio.h>
#include <stdlib.h>

/*  This should probably be in a header file */
#define MEMORY 131072 // virtual memory size in bytes, size is equal to (16^4)*2
#define MAGIC_NUM 0x7962 // y86 signifier

typedef enum { false, true } bool;

int loadAndValidate(unsigned char* buffer, unsigned long length);
void printBuffer(unsigned char* buffer, unsigned long length);
void loadIntoMemory (unsigned char* buffer, unsigned short byte_count,
                        unsigned short loadAddress);
void initMemory(unsigned int size);

struct Block{ //TODO
    unsigned short load_address;
    unsigned short byte_count;
    unsigned char* data;
} Block;

static char* memPtr; //TODO this should probably be hidden in another file
static bool memoryHasInitialized;

/* END HEADER */

int main (int argc, char* argv[]) 
{
 
    FILE* fp = fopen(argv[1], "rb"); // open the file based on command line argument
    unsigned char* buffer;

    if (fp == NULL)
        perror("Error opening file");
    else
    {
        long lSize; // Size of file
        fseek(fp, 0, SEEK_END); // find the end of the file
        lSize = ftell(fp); // save the size of the file
        rewind(fp); // go back to beginning of file
        buffer = (unsigned char*) malloc(sizeof(char)*lSize);

        if (buffer == NULL) { perror("Memory error");}

        fread(buffer, 2, lSize, fp); // read in the entire file to buffer

        memoryHasInitialized = false;
        initMemory(MEMORY);
        if (loadAndValidate(buffer, lSize) == EXIT_FAILURE){
            printf("The Y86 object file is corrupted\n");
            exit(EXIT_FAILURE);
        }
        printBuffer(buffer, lSize);
    }

    //Clean up
    fclose(fp);
    free(buffer);
    free (memPtr);
    return EXIT_SUCCESS; 
}
//TODO read file directly into memory without using the buffer first.  this way
//i don't have to go thru the file twice

void initMemory(unsigned int size)
{
    memPtr = (char*)calloc(size, 1); // Allocate 'size' bytes
    if (memPtr == NULL) {
        perror("memory allocation failed");
    }
    else
        memoryHasInitialized = true;
}

/*
 * Checks to see if the data is valid for Y86 
 *
 */
int loadAndValidate(unsigned char* buffer, unsigned long length) {


    // Check for the magic number first
    if (length < 7){ //magic number can't possibly exist, file is too short
        printf("The file is not a Y86 object file\n");
        exit(EXIT_FAILURE);
    }
    unsigned short mNum = (unsigned short)buffer[0] << 8 | buffer[1];
    if (mNum != MAGIC_NUM) {
        printf("The file is not a Y86 object file\n");
        exit(EXIT_FAILURE);
    }

    // begin checking blocks
    int i = 2; // Start at 2 since magic number has been read
    while (i + 3 < length) {
        unsigned short la = (buffer[i] << 8) + buffer[i+1];
        //unsigned short la = (unsigned short*)buffer[0];
        unsigned short byte_count = (buffer[i+2] << 8 ) + buffer[i+3];

        //TODO printf("load_address: 0x%.4x\nbyteCount: 0x%.4x\n", la, byte_count);

        i += 4;

        if (i+byte_count-1 >= length){
            printf("The Y86 object file is corrupted\n");
            exit(EXIT_FAILURE);
        }
        loadIntoMemory(buffer+i, byte_count, la);

        i += byte_count;
    }
    return EXIT_SUCCESS;

}
/*
 * pre: memory has been allocated
 *TODO pre
 * load bytes into memory consecutively
 */
void loadIntoMemory (unsigned char* buffer, unsigned short byte_count,
                        unsigned short loadAddress)
{
    unsigned char* ptr = memPtr;
    int i;
    for (i = 0; i < byte_count; i++) {
        ptr = (char*)(memPtr+loadAddress+i);
        *ptr = buffer[i];
        /*printf("loaded: 0x%.2x at 0x%.4x (0x%.2x)\n", buffer[i],
                                            loadAddress+i, ptr);
        */
    }

}

void printBuffer(unsigned char* buffer, unsigned long length)
{
    int i;

   /* 
    printf("file size: %lu bytes\n", length);
    for (i = 0; i < length; i++)
        printf(" %.2d", i);
    printf("\n");
    for (i = 0; i < length; i++)
        printf(" %.2x", buffer[i]);
    printf("\n");
   */ 

    // actually print the memory in the correct format
    for (i = 0; i < MEMORY; i++) {
        if (*(memPtr+i) != 0)
            printf("%.4X: %.2X\n", (unsigned short)i, *(unsigned char*)(memPtr+i));
    }
}

unsigned char readAddress (unsigned short loadAddress) {
    return *(unsigned char*)(memPtr+loadAddress);

}
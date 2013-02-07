/* UTEID: mp25234 program 2 */
#include <stdio.h>
#include <stdlib.h>

/*  This should probably be in a header file */
#define MEMORY  131072 // virtual memory size in bytes, size is equal to (16^4)*2
#define MAGIC_NUM  0x7962 // y86 signifier

int validateBuffer(unsigned char* buffer, long length);
void printBuffer(unsigned char* buffer, long length);

struct Block{ //TODO
    unsigned short load_address;
    unsigned short byte_count;
    unsigned char* data;
} Block;

 
static void* memPtr; //TODO this should probably be hidden in another file

/* END HEADER */

int main (int argc, char* argv[]) 
{
 
    FILE* fp = fopen(argv[1], "rb"); // open the file based on command line argument

    if (fp == NULL)
        perror("Error opening file");
    else
    {
        long lSize; // Size of file
        fseek(fp, 0, SEEK_END); // find the end of the file
        lSize = ftell(fp); // save the size of the file
        rewind(fp); // go back to beginning of file
        unsigned char* buffer = (unsigned char*) malloc(sizeof(char)*lSize); //size of char is 1 byte by definition

        if (buffer == NULL) { perror("Memory error");}

        fread(buffer, 2, lSize, fp); // read in the entire file to buffer
        //TODO where would i use the Block structure?

        validateBuffer(buffer, lSize);
        printBuffer(buffer, lSize);
    }

    printf("\n");
    fclose(fp);
    //free(buffer);
    return 0;
}

/*
 * Checks to see if the data is valid for Y86 
 */
int validateBuffer(unsigned char* buffer, long length) {

    if (length < 2){ //magic number can't possibly exist
        perror("Magic number is incorrect, this file is not of Y86 format.");
        exit(0);
    }

    // Check for the magic number first
    unsigned short mNum = (unsigned short)buffer[0] << 8 | buffer[1];
    if (mNum != MAGIC_NUM) {
        perror("Magic number does not match, this file is not of Y86 format.");
        exit(0);
    }

}
/*
 * pre: memory has been allocated
 * load bytes into memory consecutively
 */
void loadIntoMemory (unsigned char* buffer, long length, unsigned short loadAddress)
{
    int i;
    for (i = 0; i < length; i++)
        *(unsigned char)memPtr[loadAddress+i] = *(char*)buffer[i];// TODO is this correct
}

void printBuffer(unsigned char* buffer, long length)
{
    int i;
    printf("file size: %lu bytes\n", length);

    for (i = 0; i < length; i++)
        printf(" %.2d", i);
    printf("\n");
    for (i = 0; i < length; i++)
        printf(" %.2x", buffer[i]);
    printf("\n");
    // begin checking blocks
    i = 2;
    while (i + 3 < length) {
        unsigned short la = (buffer[i] << 8) + buffer[i+1];
        //unsigned short la = (unsigned short*)buffer[0];
        unsigned short byte_count = (buffer[i+2] << 8 ) + buffer[i+3];

        printf("load_address: 0x%.4x\nbyteCount: 0x%.4x\n", la, byte_count);

        i += 3;

        int j;
        for (j = 1; j <= byte_count; j++)
        {
            printf(" %.2x ", buffer[i+j]);
        }
        printf("\n");

        i += 1 + byte_count;
        printf("i updated to byte: %.2d\n", i);
    }
}

// make sure if there's a byte count of 3 the file doesn't end befor
// compare each byte before loading into virtual memory

void initMemory(unsigned int size)
{
    memPtr = calloc (size, 1); // Allocate 'size' bytes
}

//TODO read from address
unsigned char readAddress (unsigned short loadAddress) {


}

//TODO write to an addres

//TODO shut memory down

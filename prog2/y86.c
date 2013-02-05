/* UTEID: mp25234 program 2 */
#include <stdio.h>
#include <stdlib.h>

#define MEMORY  10000 // virtual memory size in bytes
#define MAGIC_NUM  0x7962 // y86 signifier

int validateBuffer(unsigned char* buffer, long length);
void printBuffer(unsigned char* buffer, long length);

typedef unsigned short int load_address;

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
        unsigned char *buffer = (unsigned char*) malloc(sizeof(char)*lSize); //size of char is 1 byte by definition

        if (buffer == NULL) { perror("Memory error");}

        fread(buffer, 2, lSize, fp); // read in the entire file to buffer

        printBuffer(buffer, lSize);
        validateBuffer(buffer, lSize);

    }

    printf("\n");
    fclose(fp);

}

/*
 * Checks to see if the data is valid for Y86 
 */
int validateBuffer(unsigned char* buffer, long length) {

        // Check for the magic number first
        unsigned short int mNum = (buffer[0] << 8) + buffer[1];
        if (mNum != MAGIC_NUM)
            printf("mNum: %.4x\nMAGIC:%.4x\n", mNum, MAGIC_NUM);
        else
            printf("Magic Number Found Validated!\n");

        // begin checking blocks
        int i = 2;
        while (i + 3 < length) {
            load_address la = (buffer[i] << 8) + buffer[i+1];
            unsigned short int byte_count = (buffer[i+2] << 8 ) + buffer[i+3];

            printf("load_address: 0x%.4X\nbyteCount: 0x%.4X\n", la, byte_count);

            i += 3;

            int j;
            for (j = 1; j <= byte_count; j++)
            {
                printf(" %.2x ", buffer[i+j]);
            }
            printf("\n");

            i += 1 + byte_count;
            printf("i updated to: 0x%.2X\n", i);
        }



}

void printBuffer(unsigned char* buffer, long length)
{
        int i;
        printf("file size: %lu bytes\n", length);

        for (i = 0; i < length; i++)
        {
            printf(" %.2x", buffer[i]);
        }
        printf("\n");


}


//TODO create a memory method to store the data in memory according to the load
//addresses


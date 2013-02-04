/* UTEID: mp25234 program 2 */
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[]) 
{
 
    // open file
    //
    // read file
    //
    // save file to memory
    //
    // print file
    //
    // Even though you sorted it out, I'll post an answer for others...use
    // unsigned variables. The first byte you read will need to be signed (an
    // int or short) so you can test for EOF. Afterwards, either cast, or use
    // unsigned types.

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

        

        int i;
        printf("file size: %lu bytes\n", lSize);
        if (buffer[0] == '0x79' && buffer[1] == '0x62')
            printf("Magic number confirmed!\n");

        for (i = 0; i < lSize; i++)
        {
            printf("%02x\n", buffer[i]);
        }
        
        
    }

    printf("\n");
    fclose(fp);

}


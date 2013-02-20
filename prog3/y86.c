/* UTEID: mp25234 program 2 */
#include "y86.h"

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
        printMemory();
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
        //unsigned short la = (unsigned short*)buffer[i];
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
    // Update minimum and maximum load addresses used
    if (loadAddress < minAddress) {
        minAddress = loadAddress;
    }
    if (loadAddress + byte_count - 1 > maxAddress) {
        maxAddress = loadAddress + byte_count -1;
    }

}

void printMemory(void) {
    int i;
    // actually print the memory in the correct format
    for (i = minAddress; i <= maxAddress; i++) {
        unsigned int littleEndian;
        
        // Form little Endian from big Endian
        littleEndian = readAddress(i) | 
                       readAddress(i+1) << 8 | 
                       readAddress(i+2) << 16 |
                       readAddress(i+3) << 24;

        printf("%04X %02X %08X %11d %s\n", i, littleEndian & 0xFF, littleEndian, 
                littleEndian, getOpcode(littleEndian & 0xFF));
    }
}

unsigned int readAddress (unsigned short loadAddress) {
    return (unsigned int)memPtr[loadAddress]; 
}

char* getOpcode(int opcode) {
    switch (opcode) {
        case 0x0: return("halt");
        case 0x10: return("nop");
        case 0x20: return("rrmovl");
        case 0x30: return("irmovl");
        case 0x40: return("rmmovl");
        case 0x50: return("mrmovl");
        case 0x80: return("call");
        case 0x90: return("ret");
        case 0xA0: return("pushl");
        case 0xB0: return("popl");
        case 0x21: return("cmovle");
        case 0x22: return("cmovl");
        case 0x23: return("cmove");
        case 0x24: return("cmovne");
        case 0x25: return("cmovge");
        case 0x26: return("cmovg");
        case 0x60: return("addl");
        case 0x61: return("subl");
        case 0x62: return("andl");
        case 0x63: return("xorl");
        case 0x70: return("jmp");
        case 0x71: return("jle");
        case 0x72: return("jl");
        case 0x73: return("je");
        case 0x74: return("jne");
        case 0x75: return("jge");
        case 0x76: return("jg");
        default: return("invalid");
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SYMBOL_LENGTH 256;
    
typedef struct {
    unsigned int inDegree;
    char* symbolName;
    struct symbol *nextSymbols;
}symbol;

void setSymbolName (symbol *sym, char *name);
char* readNextSymbolPair (FILE *fp);

int main (int argc, char* argv[]) {

    FILE* fp = fopen(argv[1], "r");
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);

    /*
    symbol *root;
    symbol *currentSymbol;
    root = malloc (sizeof(symbol));
    currentSymbol = root;

    if (currentSymbol != 0) {
        while (currentSymbol->nextSymbol != 0) {
            printf ("%d\n", currentSymbol->inDegree);
        }
    }

    setSymbolName(root, "root");
    root->nextSymbol = malloc(sizeof(symbol));
    */

    fclose(fp);
}

void setSymbolName (symbol *sym, char *name) {
    sym->symbolName = malloc(sizeof(name) +1);
    if (sym->symbolName == NULL)
        printf("error mallocing symbolName");
    strcpy(sym->symbolName, name);
}

/*
 * Reads a line and stores the symbols and dependencies into the table
 * while checking for correct format.
 */
char* readNextSymbolPair (FILE* fp) {
    if (fp == NULL)
        perror("Error opening file");

    //initial buffer size
    int total_length = DEFAULT_SYMBOL_LENGTH;
    int current_length = 0;
    char* buffer = malloc(sizeof(char)*total_length+1);

    int c = fgetc(fp); 
    while (c != EOF && c!='\n') {
        if (c == ' ') { // start a new symbol
            // first symbol has been read into the buffer.  store this in the
            // structure table now
            //TODO store in symbol table
            #ifdef DEBUG
                printf ("symbol: %s\n", buffer);
            #endif
        }
        else {
            if (current_length < total_length) {
                buffer[current_length++] = c; // increment current_length after storing c
            }
            else { // buffer has run out of space
                total_length *= 2; // double size of buffer
                void* tempBuff = realloc(buffer, total_length*sizeof(char)+1);
                if (tempBuff == NULL) {
                    free(buffer);
                    perror("Error reallocating");
                }
                buffer = (char*)tempBuff;   
                buffer[current_length++] = c;
            }
        }
        c = fgetc(fp);
    }
    #ifdef DEBUG
        printf ("symbol: %s\n", buffer);
    #endif
}   



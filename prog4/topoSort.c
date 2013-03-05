#include "topoSort.h"

int main (int argc, char* argv[]) {

    FILE* fp = fopen(argv[1], "r");
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);

    symbolTable* symbols = malloc(sizeof(symbolTable));

    fclose(fp);
}

symbol* getSymbol(symbol *root, char* name) {
    symbol *currentSymbol = root;
    while (currentSymbol != NULL) { 
        if (currentSymbol->symbolName == name)
            return currentSymbol;
        else if (currentSymbol->nextSymbol != NULL)
            currentSymbol = currentSymbol->nextSymbol;
    }
    
    // Symbol wasn't found
    return NULL;
}

void setSymbolName (symbol *sym, char *name) {
    sym->symbolName = malloc(sizeof(name) +1);
    if (sym->symbolName == NULL)
        printf("error mallocing symbolName");
    strcpy(sym->symbolName, name);
}

void addSymbolToTable (symbol *sym) {


}

/*
 * Reads a line and stores the symbols and dependencies into the table
 * while checking for correct format.
 */
void readNextSymbolPair (FILE* fp) {
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

            symbol* sym = malloc(sizeof(symbol));
            sym->inDegree=0;
            setSymbolName(sym, buffer);
            // if buffer !exists in symbol table
            addSymbolToTable(sym);

             

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
        
    free(buffer);
}   

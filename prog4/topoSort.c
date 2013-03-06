#include "topoSort.h"

static symbolTable* symbols;

int main (int argc, char* argv[]) {

    FILE* fp = fopen(argv[1], "r");
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);
    readNextSymbolPair(fp);
    
    symbols = malloc(sizeof(symbolTable));

    fclose(fp);
    free(symbols);
}

symbol* getSymbol(symbol *root, char* name) {
    symbol *currentSymbol = root;
    while (currentSymbol != NULL) { 
        if (currentSymbol->symbolName == name)
            return currentSymbol;
        else if (currentSymbol->next != NULL)
            currentSymbol = currentSymbol->next;
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

symbol* add_symbol_to_table (char* buffer) {

    // Find or create the symbol as needed
    symbol* sym = symbols->first;
    while (sym != NULL) {
        if (sym->symbolName == buffer)
            break;         
        sym = sym->next;
    }
    if (sym == NULL) {
        // Actually create the symbol from the buffer
        sym = malloc(sizeof(symbol));
        if (sym == NULL) {
            printf("error mallocing symbol");
            exit (EXIT_FAILURE);
        }
        sym->inDegree=0;
        setSymbolName(sym, buffer);
        //update symbol.next
    }

    // Adjust the symbolTable pointers
    if (symbols->first == NULL) {
       symbols->first = sym; 
       symbols->last = sym;
    }
    else {
        symbols->last->next = sym;
        symbols->last = sym;
    }
    
    return sym;
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
    if (buffer == NULL)
        exit(EXIT_FAILURE);
    bool foundFirst = false;
    symbol* firstSymbol;
    symbol* secondSymbol;

    int c = fgetc(fp); 
    while (c != EOF && c!='\n') {
        if (c == ' ') {
            if (foundFirst){ // A symbol has already been found
                perror("Too many input symbols on line");
                exit(EXIT_FAILURE);
            }

            firstSymbol = add_symbol_to_table(buffer);
            // Reset buffer for second symbol
            free(buffer);
            buffer = malloc(sizeof(char)*total_length+1);
            if (buffer == NULL)
                exit(EXIT_FAILURE);
            current_length = 0;
            foundFirst = true;
            
            #ifdef DEBUG
                printf ("first symbol: %s\n", firstSymbol->symbolName);
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

    secondSymbol = add_symbol_to_table(buffer);
    free(buffer);
    //TODO addSymbolAfter(firstSymbol, secondSymbol);
        // must check to see if secondSymbol exists there already
    secondSymbol->inDegree++;

    #ifdef DEBUG
        printf ("second symbol: %s\n", secondSymbol->symbolName);
    #endif
        
}   

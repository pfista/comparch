#include "topoSort.h"

static symbolTable* symbols;

int main (int argc, char* argv[]) {

    FILE* fp = fopen(argv[1], "r");
    // Create the symbol table
    symbols = malloc(sizeof(symbolTable));
    symbols->first = NULL;
    symbols->last = NULL;

    read_file(fp);

    symbol* current;
    for (current = symbols->first; current != NULL; current = current->next) {
        printf("Name: %s\n", current->symbolName);
        printf("In-Degree: %d\n", current->inDegree);
        symbolsAfter* curA;
        for (curA = current->curSymbolAfter; curA != NULL; curA = curA->nextSymAfter) {
            printf("\tSymbol After: %s\n", curA->sym->symbolName);
        }
        printf("^^\n\n");
    }

    fclose(fp);
    free(symbols);
}

symbol* getSymbol(char* name) {
    symbol* currentSymbol = symbols->first;
    while (currentSymbol != NULL) { 
        if (strcmp(currentSymbol->symbolName,name) == 0) {
            return currentSymbol;
        }
        currentSymbol = currentSymbol->next;
    }
    // Symbol wasn't found
    return NULL;
}

void setSymbolName (symbol *sym, char *name) {
    #ifdef DEBUG
        printf ("copying from buffer to alloced symbol: %s\n", name);
    #endif
    sym->symbolName = malloc(sizeof(name) + 1);
    if (sym->symbolName == NULL)
        printf("error mallocing symbolName");
    strcpy(sym->symbolName, name);
}

symbol* init_symbol() {
    symbol* sym = malloc(sizeof(symbol));
    if (sym == NULL) {
        printf("error mallocing symbol");
        exit (EXIT_FAILURE);
    }
    sym->inDegree = 0;
    sym->symbolName = NULL;
    sym->curSymbolAfter = NULL;
    sym->next = NULL;
    return sym;
}

symbolsAfter* init_symbol_after() {
    symbolsAfter* symA = malloc(sizeof(symbolsAfter));
    if (symA == NULL) {
        printf("error mallocing symbol");
        exit (EXIT_FAILURE);
    }
    symA->sym = NULL;
    symA->nextSymAfter = NULL;
    return symA;
}

symbol* add_symbol_to_table (char* buffer) {
    symbol* sym = NULL;
    #ifdef DEBUG
        printf ("adding symbol with buffer: %s\n", buffer);
    #endif

    // Find or create the symbol as needed
    if (symbols->first != NULL) {
        sym = getSymbol(buffer); 
    }
    if (sym == NULL) { // Symbol wasn't found
        // Actually create the symbol from the buffer
        sym = init_symbol();
        setSymbolName(sym, buffer);
    }
    else {
        // Don't need to update table, return early since sym has been found
        return sym;
    }

    // Adjust the symbolTable pointers
    if (symbols->first == NULL) {
        symbols->first = sym; 
        symbols->last = sym;
        symbols->first->next = NULL;
    }
    else {
        symbols->last->next = sym; // Point the previous last's next to the new last
        symbols->last = sym;
    }
    return sym;
}

void add_symbol_after (symbol* symb, symbol* symAfter) {
    // if listOfsymbo
    if (symb->curSymbolAfter == NULL) {
        symb->curSymbolAfter = init_symbol_after();
        symb->curSymbolAfter->sym = symAfter;
    }
    else {
        sym_after_exists(symb, symAfter);
        symbolsAfter* newSymAfter = init_symbol_after(); // Create a "symbolAfter" struct
        newSymAfter->sym = symAfter; // Set the "symbolAfter" to point to the "symbol" that comes after
        newSymAfter->nextSymAfter = symb->curSymbolAfter; // Make the new "symbolafter" point to the head of the lis 
        symb->curSymbolAfter = newSymAfter; // Update the head of the list 
    }

}

/*
 * Check to see if a symbolsAfter exists for a symbol
 */
void sym_after_exists(symbol* symb, symbol* symAfter) {
    symbolsAfter* current = symb->curSymbolAfter;
    while (current != NULL) {
        if (current->sym == symAfter) {
            perror("This symbol already has that as a requisite");
            exit (EXIT_FAILURE);
        }
        current = current->nextSymAfter;
    }
}

void read_file(FILE* fp) {
    while (readNextSymbolPair(fp));
}

/*
 * Reads a line and stores the symbols and dependencies into the table
 * while checking for correct format.
 */
bool readNextSymbolPair (FILE* fp) {
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
    if (c == EOF) {
        free(buffer);
        return false;
    }
    while (c!='\n') {
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
    add_symbol_after(firstSymbol, secondSymbol);
    secondSymbol->inDegree++;

    #ifdef DEBUG
        printf ("second symbol: %s\n", secondSymbol->symbolName);
        printf ("second symbol indegree: %d\n", secondSymbol->inDegree);
    #endif

    return true;
        
}   

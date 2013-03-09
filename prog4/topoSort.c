#include "topoSort.h"

int main (int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "r");
    // Create the symbol table
    symbols = malloc(sizeof(symbolTable));
    symbols->first = NULL;
    symbols->last = NULL;
    symbols->size = 0;

    //Create the sortedSymbols list
    sortedSyms = malloc(sizeof(sortedSymbolList));
    sortedSyms->first = NULL;
    sortedSyms->size = 0;

    read_file(fp);
    
    /*print_symbol_table();*/
    print_topo_order(); 


    fclose(fp);
    free(symbols);
    free(sortedSyms);
}

bool print_topo_order(void) {
    // Now print the symbols in order
    symbol* current;
    while (symbols->size > 0) {
    #ifdef DEBUG
        print_symbol_table();
    #endif
        for (current = symbols->first; current != NULL; current = current->next) {
            if (current->inDegree == 0) {
                // Add to sorted list
                add_symbol_to_sorted(current);
            }
        }
        #ifdef DEBUG
            print_sorted_symbols();
        #endif
        if (sortedSyms->size == 0) {
            perror("This cannot be solved, there is a cycle");
            exit(EXIT_FAILURE);
        }
        else {
            // Now go thru sorted list, print, decrement the indegrees,  and free all the data
            sortedSymbol* currentSorted;
            for (currentSorted = sortedSyms->first; currentSorted != NULL; currentSorted = currentSorted->next) {
                printf("%s ", currentSorted->sym->symbolName);
                symbolsAfter* symA;
                for (symA = currentSorted->sym->curSymbolAfter; symA != NULL; symA = symA->nextSymAfter) {
                    /*Go thru the current symbol's "symbolsAfter" list and decrement
                     *their in-degree
                     */
                    symA->sym->inDegree--;
                }
    #ifdef DEBUG
        printf("printing new In Degrees after traversing sorted list\n");
        print_symbol_table();
    #endif
            }
            destroy_sorted_symbols();
        }
    }
    printf("\n");
}

// Frees the sorted Symbol list as well as the associated symbols in the symbol
// table
void destroy_sorted_symbols(void) {
    sortedSymbol* ssym = sortedSyms->first;
    while (ssym != NULL) {
        remove_symbol(ssym->sym);
        sortedSymbol* temp = ssym; 
        ssym = ssym->next;
        free(temp);
    }
    sortedSyms->size = 0;
    sortedSyms->first = NULL;
}

void remove_sorted_symbol() {


}

void remove_symbol(symbol* sym) {
    #ifdef DEBUG
        printf ("Removing symbol %s\n", sym->symbolName);
    #endif
    if (symbols->size > 0) {
        if (sym->prev != NULL) {
            sym->prev->next = sym->next;
        }
        if (sym->next != NULL) { 
            sym->next->prev = sym->prev;
        }
        // destroy curSymbolsAfter
        if (symbols->first == sym)
            symbols->first = sym->next;

        free(sym->symbolName);
        sym->next = NULL;
        sym->prev = NULL;
        destroy_symbols_after(sym->curSymbolAfter);
        free(sym);
        symbols->size--;
    }
    #ifdef DEBUG
        printf ("Done removing symbol\n");
    #endif

}

void destroy_symbols_after(symbolsAfter* symbolsAfter) {
    #ifdef DEBUG
        printf ("\tDestroying symbols after\n");
    #endif
    if (symbolsAfter != NULL) {
        // The sym will be destroy by remove_symbol very soon
        destroy_symbols_after(symbolsAfter->nextSymAfter);
        free(symbolsAfter);
    }
}


// TODO clean lists that frees sortedSymbol list and symbolTable

void print_symbol_table(void) {
    symbol* current;
    printf("***Symbol Table***\n");
    for (current = symbols->first; current != NULL; current = current->next) {
        printf("\tName: %s\n", current->symbolName);
        printf("\tIn-Degree: %d\n", current->inDegree);
        symbolsAfter* curA;
        for (curA = current->curSymbolAfter; curA != NULL; curA = curA->nextSymAfter) {
            printf("\t\tSymbol After: %s\n", curA->sym->symbolName);
        }
        printf("\n");
    }

}

void print_sorted_symbols(void) {
    sortedSymbol* current;
    printf("***printing Sorted Symbols***\n[ ");
    for (current = sortedSyms->first; current != NULL; current = current->next) {
        printf("%s, ", current->sym->symbolName);
    }
    printf("]\n****\n");

}

symbol* get_symbol(char* name) {
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

void set_symbol_name (symbol *sym, char *name, int length) {
    #ifdef DEBUG
        printf ("copying from buffer to alloced symbol: %s\n", name);
    #endif
    sym->symbolName = malloc(sizeof(char)*length);
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
    sym->prev = NULL;
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

symbol* add_symbol_to_table (char* buffer, int size) {
    symbol* sym = NULL;
    #ifdef DEBUG
        printf ("adding symbol with buffer: %s\n", buffer);
    #endif

    // Find or create the symbol as needed
    if (symbols->size > 0) {
        sym = get_symbol(buffer); 
    }

    if (sym == NULL) { // Symbol wasn't found
        // Actually create the symbol from the buffer
        sym = init_symbol();
        set_symbol_name(sym, buffer, size);
    }
    else {
        // Don't need to update table, return early since sym has been found
        return sym;
    }

    // Adjust the symbolTable pointers
    if (symbols->size == 0) {
        symbols->first = sym; 
        symbols->last = sym;
    }
    else {
        symbols->last->next = sym; // Point the previous last's next to the new last
        sym->prev = symbols->last;
        symbols->last = sym;
    }
    symbols->size++;
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
    while (read_next_symbol_pair(fp));
}

bool read_next_symbol_pair (FILE* fp) {
    if (fp == NULL)
        perror("Error opening file");

    //initial buffer size
    int total_length = DEFAULT_SYMBOL_LENGTH;
    int current_length = 0;
    char* buffer = malloc(sizeof(char)*total_length+1);
    if (buffer == NULL)
        exit(EXIT_FAILURE);
    bool foundFirst = false;
    bool foundBoth = false;
    bool foundThird = false;
    bool firstStarted = false;
    symbol* firstSymbol;
    symbol* secondSymbol;

    int c = fgetc(fp); 
    if (c == EOF) {
        free(buffer);
        return false;
    }
    else if (c == '\n')
        return;
    while (c!='\n') {
        if (c == ' ') {
            if (!foundFirst){ // A symbol has already been found
                // skip this character
                buffer[current_length] = '\0';
                firstSymbol = add_symbol_to_table(buffer,current_length+1);
                // Reset buffer for second symbol
                free(buffer);
                buffer = malloc(sizeof(char)*total_length);
                if (buffer == NULL)
                    exit(EXIT_FAILURE);
                current_length = 0;
                foundFirst = true;
                
                #ifdef DEBUG
                    printf ("first symbol: %s\n", firstSymbol->symbolName);
                #endif
            }
            if (foundBoth) {
                foundThird = true;
            }

        }
        else {
            if (current_length < total_length-1) {
                buffer[current_length++] = c; // increment current_length after storing c
                if (!foundFirst) {
                    firstStarted = true;
                }
                if (foundFirst)
                    foundBoth = true;
                if (foundThird) {
                    perror("found too many symbols on line\n");
                    exit(EXIT_FAILURE);
                }
            }
            else { // buffer has run out of space
                #ifdef DEBUG
                    printf ("DOUBLING BUFFER**\n");
                #endif
                total_length *= 2; // double size of buffer
                void* tempBuff = realloc(buffer, total_length*sizeof(char));
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
    // Exits if only one symbol was found on a line
    if (!foundFirst && firstStarted) {
        perror("not enough symbols on line");
        exit(EXIT_FAILURE);
    }
    else if (!firstStarted)
        return true; // Blank line with spaces only, exit

    buffer[current_length] = '\0';
    secondSymbol = add_symbol_to_table(buffer,current_length+1);
    free(buffer);
    add_symbol_after(firstSymbol, secondSymbol);
    secondSymbol->inDegree++;

    #ifdef DEBUG
        printf ("second symbol: %s\n", secondSymbol->symbolName);
        printf ("second symbol indegree: %d\n", secondSymbol->inDegree);
    #endif

    return true;
}   
void add_symbol_to_sorted(symbol* sym){
    sortedSymbol* symToAdd = init_sorted_symbol();
    symToAdd->sym = sym;
    if (sortedSyms->size == 0){
        sortedSyms->first = symToAdd;
        sortedSyms->size++;
        return;
    }
    else if (sortedSyms->size == 1) {
        if (strcmp(symToAdd->sym->symbolName, sortedSyms->first->sym->symbolName) < 0) {
            // Add at beginning of sorted list
            symToAdd->next = sortedSyms->first;
            sortedSyms->first = symToAdd;
        }
        else
            // Add to end of sorted list
            sortedSyms->first->next = symToAdd;
        sortedSyms->size++;
        return;
    }
    else {
        // Insert in the correct place to remain sorted
        sortedSymbol* current;
        sortedSymbol* prev = NULL;
        for (current = sortedSyms->first; current != NULL; current = current->next){
            if (strcmp(symToAdd->sym->symbolName, current->sym->symbolName) < 0) {
                if (prev == NULL) { // Cur at first position in list
                    symToAdd->next = current;
                    sortedSyms->first = symToAdd;
                }
                else {
                    prev->next = symToAdd;
                    symToAdd->next = current;
                }
                sortedSyms->size++;
                return;
            }
            prev = current;
        }
        // Check edge case for last spot
        if (strcmp(symToAdd->sym->symbolName, prev->sym->symbolName) > 0) {
            prev->next = symToAdd;
            sortedSyms->size++;
            return;
        }
    }

}

sortedSymbol* init_sorted_symbol() {
    sortedSymbol* ssym = malloc(sizeof(sortedSymbol));
    ssym->sym = NULL;
    ssym->next = NULL;
    return ssym;
}

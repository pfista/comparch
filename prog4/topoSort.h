#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SYMBOL_LENGTH 256;
    
typedef struct symbol {
    unsigned int inDegree;
    char* symbolName;
    struct symbolsAfter* curSymbolAfter;
    struct symbol* next;
}symbol;

typedef struct {
    symbol* first;
    symbol* last;
}symbolTable;

typedef struct symbolsAfter {
    struct symbol *sym;
    struct symbolsAfter *nextSymAfter;
}symbolsAfter;

typedef enum {false, true} bool;

void setSymbolName (symbol *sym, char *name);
void read_file(FILE *fp);
bool readNextSymbolPair (FILE *fp);
symbol* add_symbol_to_table(char* buffer);
symbolsAfter* init_symbol_after();
void sym_after_exists(symbol* symb, symbol* symAfter);
void add_symbol_after(symbol* symb, symbol* symAfter);


/*
remove a zero from top of list
    go thru symbols after
    */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SYMBOL_LENGTH 256;
    
typedef struct {
    unsigned int inDegree;
    char* symbolName;
    struct symbolsAfter *listOfSymbolsAfter;
    struct symbol *next;
}symbol;

typedef struct {
    symbol* first;
    symbol* last;
}symbolTable;

typedef struct {
    struct symbol *sym;
    struct symbolsAfter *nextSym;
}symbolsAfter;

void setSymbolName (symbol *sym, char *name);
void readNextSymbolPair (FILE *fp);


try to find a, return pointer to it if exists else
if (!A exists)
    add symbol to list

try to find b, return pointer to it if exists else
if (!B exists)
    add symbol to list

A->addBtoSymbolsAfterList
B->inDegree++
    


add symbol to list
    malloc(symbol)
    setLast -> newSymbol

addBtoSymbolsAfterList
    malloc(symbolsAfter)
    set sym* -> B
    

remove a zero from top of list
    go thru symbols after

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SYMBOL_LENGTH 4;
    
typedef struct symbol {
    unsigned int inDegree;
    char* symbolName;
    struct symbolsAfter* curSymbolAfter;
    struct symbol* next;
    struct symbol* prev;
}symbol;

typedef struct {
    symbol* first;
    symbol* last;
    int size;
}symbolTable;

typedef struct symbolsAfter {
    struct symbol *sym;
    struct symbolsAfter *nextSymAfter;
}symbolsAfter;

typedef struct sortedSymbol {
    symbol* sym;
    struct sortedSymbol* next;
}sortedSymbol;

typedef struct sortedSymbolList {
    sortedSymbol* first;
    int size;
}sortedSymbolList;

static symbolTable* symbols;
static sortedSymbolList* sortedSyms;

typedef enum {false, true} bool;

//TODO free everything correctly
void remove_symbol(symbol* sym);
void remove_sorted_symbol(void);
void destroy_symbols_after(symbolsAfter* symbolsAfter);
void destroy_sorted_symbols(void);

bool print_topo_order(void);
void print_symbol_table(void);
void print_sorted_symbols(void);

/**
 * @brief Searches for a symbol in the symbolTable with name
 *
 * @param name - string to find
 *
 * @return symbol* if found, or NULL otherwise
 */
symbol* get_symbol(char* name);

/**
 * @brief Copies the name into memory, pointed to by sym
 *
 * @param sym - symbol to copy name into
 * @param name - string of name to copy
 * @param length - size of name
 */
void set_symbol_name (symbol *sym, char *name, int length);

/**
 * @brief Initializes a symbol with NULL values
 *
 * @return symbol* to the new object
 */
symbol* init_symbol();

/**
 * @brief Initializes a new symbolsAfter struct with NULL values
 *
 * @return symbolsAfter* to the initialized object
 */
symbolsAfter* init_symbol_after();

/**
 * @brief If the table doesn't contain the symbol name as specified in buffer,
 * a new symbol will be created and added to the table. Otherwise a pointer is
 * returned to the symbol that already exists.
 *
 * @param buffer - contains the symbol name
 * @param size - size of the buffer
 *
 * @return pointer to the symbol created (or the one already found in the
 * table)
 * 
 */
symbol* add_symbol_to_table(char* buffer, int size);

/**
 * @brief Adds a symbol to the symbolsAfter list inside of symb
 *
 * @param symb
 * @param symAfter
 */
void add_symbol_after(symbol* symb, symbol* symAfter);

/**
 * @brief Checks to see if a symbolAfter exists for the current symbol
 *
 * @param symb - symbol checked
 * @param symAfter - tries to find symAfter in symb's curSymbolAfter list
 */
void sym_after_exists(symbol* symb, symbol* symAfter);

/**
 * @brief Method that drives the read_next_symbol_pair function
 *
 * @param fp FILE* pointing to the file to read
 */
void read_file(FILE *fp);

/**
 * @brief Reads a line and stores the symbols and dependencies into the table
 * while checking for the correct format
 *
 * @param fp filepointer to the txt file that is being read in
 *
 * @return false if the EOF has been reached, true if the symbol pair has been
 * correctly read and added to the table
 */
bool read_next_symbol_pair (FILE *fp);

sortedSymbol* init_sorted_symbol(void);
void add_symbol_to_sorted(symbol* sym);




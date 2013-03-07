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

void print_symbol_table();

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
 */
void set_symbol_name (symbol *sym, char *name);

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
 *
 * @return pointer to the symbol created (or the one already found in the
 * table)
 */
symbol* add_symbol_to_table(char* buffer);

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


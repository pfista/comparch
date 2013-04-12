/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

#ifndef _Y86_H_
#define _Y86_H_ 1

/* The global environment for all Y86 simulator files */

#include <stdio.h>
#include <stdlib.h>

typedef short Boolean;
#define TRUE 1
#define FALSE 0

typedef char *STRING;

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* Y86 address is 32-bit integer */
typedef unsigned int y86_address;

/* Y86 register is 32-bit integer */
typedef unsigned int y86_register;
typedef unsigned char reg_num;

/* Memory is byte-addressable */
typedef unsigned char byte;


#include "y86_opcodes.h"
#include "y86_decode.h"
#include "y86_exe.h"


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* debug flag */
Boolean debug;
Boolean CC_print;

Boolean halted;
y86_address PC;
y86_register GP[8];

Boolean ZF;
Boolean SF;
Boolean OF;

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

const STRING reg_name[16];

y86_register get_Register(reg_num r);
void set_Register(reg_num r, y86_register v);
void set_PC(y86_address d);

void Part2Trace(STRING s);

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* In case we have an error */
void error_exit(const STRING message);


/* prototypes for accessing memory */
enum read_type { DATA_READ, DATA_STORE, INSTRUCTION_FETCH };

void init_memory(void);
void set_memory(y86_address a, byte value);
byte get_memory(y86_address a);

void set_memory_4(y86_address a, y86_register value, enum read_type line);
y86_register get_memory_4(y86_address a, enum read_type line);

#endif


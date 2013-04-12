/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* prototypes for decoding */

#ifndef _Y86_DECODE_H_
#define _Y86_DECODE_H_ 1

#include "y86.h"

struct decode_table
{
    STRING name;
    byte length;
    byte number;
};


struct decode_table *y86_decode(int opcode);


#endif


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

#include "y86.h"
#include "y86_decode.h"

/* Y86 Decode table */

struct decode_table DT[256] = {{0}};

void enter_decode_table(const STRING name, int opcode, int number, int length)
{
    struct decode_table *dte = &DT[opcode];
    dte->name = name;
    dte->number = number;
    dte->length = length;
}

void initialize_decode_table(void)
{
    enter_decode_table("halt", 0x00, OP_HALT, 1);
    enter_decode_table("nop", 0x10, OP_NOP, 1);
    enter_decode_table("irmovl", 0x30, OP_IRMOVL, 6);
    enter_decode_table("rmmovl", 0x40, OP_RMMOVL, 6);
    enter_decode_table("mrmovl", 0x50, OP_MRMOVL, 6);
    enter_decode_table("call", 0x80, OP_CALL, 5);
    enter_decode_table("ret", 0x90, OP_RET, 1);
    enter_decode_table("pushl", 0xA0, OP_PUSHL, 2);
    enter_decode_table("popl", 0xB0, OP_POPL, 2);
    
    enter_decode_table("rrmovl", 0x20, OP_RRMOVL, 2);
    enter_decode_table("cmovle", 0x21, OP_CMOVLE, 2);
    enter_decode_table("cmovl", 0x22, OP_CMOVL, 2);
    enter_decode_table("cmove", 0x23, OP_CMOVE, 2);
    enter_decode_table("cmovne", 0x24, OP_CMOVNE, 2);
    enter_decode_table("cmovge", 0x25, OP_CMOVGE, 2);
    enter_decode_table("cmovg", 0x26, OP_CMOVG, 2);
    
    enter_decode_table("addl", 0x60, OP_ADDL, 2);
    enter_decode_table("subl", 0x61, OP_SUBL, 2);
    enter_decode_table("andl", 0x62, OP_ANDL, 2);
    enter_decode_table("xorl", 0x63, OP_XORL, 2);
    
    enter_decode_table("jmp", 0x70, OP_JMP, 5);
    enter_decode_table("jle", 0x71, OP_JLE, 5);
    enter_decode_table("jl", 0x72, OP_JL, 5);
    enter_decode_table("je", 0x73, OP_JE, 5);
    enter_decode_table("jne", 0x74, OP_JNE, 5);
    enter_decode_table("jge", 0x75, OP_JGE, 5);
    enter_decode_table("jg", 0x76, OP_JG, 5);
}

struct decode_table *y86_decode(int opcode)
{
    static Boolean initialized = FALSE;
    
    if (!initialized)
        {
            initialize_decode_table();
            initialized = TRUE;
        }
    return(&DT[opcode]);
}

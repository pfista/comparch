/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

#ifndef _Y86_EXE_H_
#define _Y86_EXE_H_ 1

#include "y86.h"    /* need decode_table structure definition */

/* prototypes for each of the functions for each instructions */

void exe_halt(y86_address pc, struct decode_table *dte);
void exe_nop(y86_address pc, struct decode_table *dte);
void exe_irmovl(y86_address pc, struct decode_table *dte);
void exe_rmmovl(y86_address pc, struct decode_table *dte);
void exe_mrmovl(y86_address pc, struct decode_table *dte);
void exe_call(y86_address pc, struct decode_table *dte);
void exe_ret(y86_address pc, struct decode_table *dte);
void exe_pushl(y86_address pc, struct decode_table *dte);
void exe_popl(y86_address pc, struct decode_table *dte);
void exe_rrmovl(y86_address pc, struct decode_table *dte);
void exe_cmovle(y86_address pc, struct decode_table *dte);
void exe_cmovl(y86_address pc, struct decode_table *dte);
void exe_cmove(y86_address pc, struct decode_table *dte);
void exe_cmovne(y86_address pc, struct decode_table *dte);
void exe_cmovge(y86_address pc, struct decode_table *dte);
void exe_cmovg(y86_address pc, struct decode_table *dte);
void exe_addl(y86_address pc, struct decode_table *dte);
void exe_subl(y86_address pc, struct decode_table *dte);
void exe_andl(y86_address pc, struct decode_table *dte);
void exe_xorl(y86_address pc, struct decode_table *dte);
void exe_jmp(y86_address pc, struct decode_table *dte);
void exe_jle(y86_address pc, struct decode_table *dte);
void exe_jl(y86_address pc, struct decode_table *dte);
void exe_je(y86_address pc, struct decode_table *dte);
void exe_jne(y86_address pc, struct decode_table *dte);
void exe_jge(y86_address pc, struct decode_table *dte);
void exe_jg(y86_address pc, struct decode_table *dte);

#endif


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

#include "y86.h"
#include <string.h>

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* code for setting, and checking, the condition codes */

#define sign_bit(v)    ((v >> 31) & 0x1)

void FormatCC(STRING buffer)
{
    sprintf(buffer, "%c%c%c",
            ZF ? 'Z' : '.', 
            SF ? 'S' : '.', 
            OF ? 'O' : '.');
}

void set_flags(y86_register val_new, int sign_A, int sign_B)
{
    /* zero flag if the new value is zero */
    ZF = (val_new == 0);

    /* sign flag if the new value is negative -- sign bit is one */
    SF = sign_bit(val_new);
    
    /* overflow flag is if the signs of the input (sign_A and sign_B) are 
       the same, and different from the result (val_new) */
    if (sign_A != sign_B)
        OF = 0;
    else if (sign_A == sign_bit(val_new))
        OF = 0;
    else
        OF = 1;
    if (debug) fprintf(stdout, "ZF=%d, SF=%d, OF=%d\n", ZF, SF, OF);

    char buffer[32];
    FormatCC(buffer);
    strcat(buffer, " -> CC");
    Part2Trace(buffer);
}

Boolean eval_cc_eq(void)
{
    /* jump if equal means ZF is 1 */
    char buffer[32];
    sprintf(buffer, "ZF->%d", ZF);
    Part2Trace(buffer);
    return(ZF);
}

Boolean eval_cc_l(void)
{
    /* jump if equal means SF is not OF.  SF is negative (less than) */
    char buffer[32];
    sprintf(buffer, "SF->%d, OF->%d", SF, OF);
    Part2Trace(buffer);
    return(SF != OF);
}

Boolean eval_cc_le(void)
{
    /* less or equal is (1) equal or (2) less */
    if (eval_cc_eq()) return(TRUE);
    return(eval_cc_l());
}

int PrintCC(void)
{
    char buffer[32];
    FormatCC(buffer);
    fprintf(stdout, buffer);
    return(strlen(buffer));
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* code for printing the output */
/* output is several different parts:

   1. PC, opcode
   2. Condition Codes
   3. What was used and set by the instruction.

   PrintPart1 does 1 and 2.
   PrintPart2 does 3.

*/

int output_column = 0;
char part2_string[1024];


void PrintPart1(y86_address pc, struct decode_table *dte, STRING operands)
{
    if (CC_print)
        {
            output_column += PrintCC();
        }
    output_column += fprintf(stdout, "0x%08X: %s %s", pc, dte->name, operands);
}

void Part2Trace(STRING s)
{
    if (strlen(part2_string) != 0)
        {
            strcat(part2_string, ", ");
        }
    strcat(part2_string, s);
}


void PrintPart2(void)
{
    if (strlen(part2_string) != 0)
        {
            /* space over to column 50 */
            while (output_column < 49) 
                {
                    fprintf(stdout, " ");
                    output_column += 1;
                }
            fprintf(stdout, "(%s)", part2_string);
        }
    fprintf(stdout, "\n");

    part2_string[0] = '\0';
    output_column = 0;
}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

#define extract_rA(v)  ((v >> 4) & 0xF)
#define extract_rB(v)  ((v >> 0) & 0xF)

char operands[32];




/* the functions that execute each instruction */

void exe_halt(y86_address pc, struct decode_table *dte)
{
    halted = TRUE;
    operands[0] = '\0';
    PrintPart1(pc, dte, operands);
    PrintPart2();
}

void exe_nop(y86_address pc, struct decode_table *dte)
{
    /* nothing to do for a nop instruction */
    operands[0] = '\0';
    PrintPart1(pc, dte, operands);
    PrintPart2();
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void exe_irmovl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rB = extract_rB(regs);
    y86_register v = get_memory_4(pc+2, INSTRUCTION_FETCH);
    sprintf(operands, "0x%08X,%s",v, reg_name[rB]);
    PrintPart1(pc, dte, operands);

    set_Register(rB, v);
    PrintPart2();
}

void exe_rmmovl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    reg_num rB = extract_rB(regs);
    y86_address d = get_memory_4(pc+2, INSTRUCTION_FETCH);
    sprintf(operands, "%s,0x%08X(%s)", reg_name[rA], d, reg_name[rB]);
    PrintPart1(pc, dte, operands);

    y86_address dst = get_Register(rB) + d;
    set_memory_4(dst, get_Register(rA), DATA_STORE);
    PrintPart2();
}

void exe_mrmovl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    reg_num rB = extract_rB(regs);
    y86_address s = get_memory_4(pc+2, INSTRUCTION_FETCH);
    sprintf(operands, "0x%08X(%s),%s", s, reg_name[rB], reg_name[rA]);
    PrintPart1(pc, dte, operands);

    y86_address src = get_Register(rB) + s;
    y86_register val = get_memory_4(src, DATA_READ);
    set_Register(rA, val);
    PrintPart2();
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void exe_call(y86_address pc, struct decode_table *dte)
{
    y86_address dest = get_memory_4(pc+1, INSTRUCTION_FETCH);
    y86_address sp_add = get_Register(ESP) - 4;
    sprintf(operands, "0x%08X", dest);
    PrintPart1(pc, dte, operands);

    set_memory_4(sp_add, PC, DATA_STORE);
    set_Register(ESP, sp_add);
    set_PC(dest);
    PrintPart2();
}

void exe_ret(y86_address pc, struct decode_table *dte)
{
    y86_address sp_add = get_Register(ESP);
    operands[0] = '\0';
    PrintPart1(pc, dte, operands);

    y86_address dest = get_memory_4(sp_add, DATA_READ);
    set_Register(ESP, sp_add + 4);
    set_PC(dest);
    PrintPart2();
}

void exe_pushl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    sprintf(operands, "%s", reg_name[rA]);
    PrintPart1(pc, dte, operands);

    y86_register val = get_Register(rA);
    y86_address sp_add = get_Register(ESP) - 4;
    set_memory_4(sp_add, val, DATA_STORE);
    set_Register(ESP, sp_add);
    PrintPart2();
}

void exe_popl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    sprintf(operands, "%s", reg_name[rA]);
    PrintPart1(pc, dte, operands);

    y86_address sp_add = get_Register(ESP);
    y86_register val = get_memory_4(sp_add, DATA_READ);
    set_Register(ESP, sp_add + 4);
    set_Register(rA, val);
    PrintPart2();
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void exe_addl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    reg_num rB = extract_rB(regs);
    sprintf(operands, "%s,%s", reg_name[rA], reg_name[rB]);
    PrintPart1(pc, dte, operands);

    y86_register vA = get_Register(rA);
    y86_register vB = get_Register(rB);
    y86_register vE = vB + vA;
    set_Register(rB, vE);
    set_flags(vE, sign_bit(vA), sign_bit(vB));
    PrintPart2();
}

void exe_subl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    reg_num rB = extract_rB(regs);
    sprintf(operands, "%s,%s", reg_name[rA], reg_name[rB]);
    PrintPart1(pc, dte, operands);

    y86_register vA = get_Register(rA);
    y86_register vB = get_Register(rB);
    y86_register vE = vB - vA;
    set_Register(rB, vE);
    set_flags(vE, sign_bit(vB), 1-sign_bit(vA));
    PrintPart2();
}

void exe_andl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    reg_num rB = extract_rB(regs);
    sprintf(operands, "%s,%s", reg_name[rA], reg_name[rB]);
    PrintPart1(pc, dte, operands);

    y86_register vA = get_Register(rA);
    y86_register vB = get_Register(rB);
    y86_register vE = vB & vA;
    set_Register(rB, vE);
    set_flags(vE, sign_bit(vE), sign_bit(vE));
    PrintPart2();
}

void exe_xorl(y86_address pc, struct decode_table *dte)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    reg_num rB = extract_rB(regs);
    sprintf(operands, "%s,%s", reg_name[rA], reg_name[rB]);
    PrintPart1(pc, dte, operands);

    y86_register vA = get_Register(rA);
    y86_register vB = get_Register(rB);
    y86_register vE = vB ^ vA;
    set_Register(rB, vE);
    set_flags(vE, sign_bit(vE), sign_bit(vE));
    PrintPart2();
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void exe_conditional_jump(y86_address pc, struct decode_table *dte, Boolean jump)
{
    y86_address dest = get_memory_4(pc+1, INSTRUCTION_FETCH);
    sprintf(operands, "0x%08X", dest);
    PrintPart1(pc, dte, operands);
    
    if (jump) 
        {
            set_PC(dest);
        }
    else
        {
            Part2Trace("no jump");
        }
    PrintPart2();
}

void exe_jmp(y86_address pc, struct decode_table *dte)
{
    exe_conditional_jump(pc, dte, TRUE);
}

void exe_jle(y86_address pc, struct decode_table *dte)
{
    Boolean jump = eval_cc_le();
    exe_conditional_jump(pc, dte, jump);
}

void exe_jl(y86_address pc, struct decode_table *dte)
{
    Boolean jump = eval_cc_l();
    exe_conditional_jump(pc, dte, jump);
}

void exe_je(y86_address pc, struct decode_table *dte)
{
    Boolean jump = eval_cc_eq();
    exe_conditional_jump(pc, dte, jump);
}

void exe_jne(y86_address pc, struct decode_table *dte)
{

    Boolean jump = !eval_cc_eq();
    exe_conditional_jump(pc, dte, jump);
}

void exe_jge(y86_address pc, struct decode_table *dte)
{
    Boolean jump = !eval_cc_l();
    exe_conditional_jump(pc, dte, jump);
}

void exe_jg(y86_address pc, struct decode_table *dte)
{
    Boolean jump = !eval_cc_le();
    exe_conditional_jump(pc, dte, jump);
}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void exe_conditional_move(y86_address pc, struct decode_table *dte, Boolean move)
{
    byte regs = get_memory(pc+1);
    reg_num rA = extract_rA(regs);
    reg_num rB = extract_rB(regs);
    sprintf(operands, "%s,%s", reg_name[rA], reg_name[rB]);
    PrintPart1(pc, dte, operands);
    
    if (!move)
        {
            Part2Trace("no move");
        }
    else
        {
            y86_register vA = get_Register(rA);
            set_Register(rB, vA);
        }
    PrintPart2();
}

void exe_rrmovl(y86_address pc, struct decode_table *dte)
{
    Boolean move = TRUE;
    exe_conditional_move(pc, dte, move);
}

void exe_cmovle(y86_address pc, struct decode_table *dte)
{
    Boolean move = eval_cc_le();
    exe_conditional_move(pc, dte, move);
}

void exe_cmovl(y86_address pc, struct decode_table *dte)
{
    Boolean move = eval_cc_l();
    exe_conditional_move(pc, dte, move);
}

void exe_cmove(y86_address pc, struct decode_table *dte)
{
    Boolean move = eval_cc_eq();
    exe_conditional_move(pc, dte, move);
}

void exe_cmovne(y86_address pc, struct decode_table *dte)
{
    Boolean move = !eval_cc_eq();
    exe_conditional_move(pc, dte, move);
}

void exe_cmovge(y86_address pc, struct decode_table *dte)
{
    Boolean move = !eval_cc_l();
    exe_conditional_move(pc, dte, move);
}

void exe_cmovg(y86_address pc, struct decode_table *dte)
{
    Boolean move = !eval_cc_le();
    exe_conditional_move(pc, dte, move);
}



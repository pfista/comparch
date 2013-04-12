/* UTCS CS429 Spring 2013 */
/* Programming Assignment 5 */

#include "y86.h"

Boolean debug = FALSE;
Boolean CC_print = FALSE;

Boolean halted = FALSE;
y86_address PC;
y86_register GP[8] = {0};

const STRING reg_name[16] = {
    "%EAX","%ECX","%EDX","%EBX","%ESP","%EBP","%ESI","%EDI",
    "invalid","invalid","invalid","invalid",
    "invalid","invalid","invalid","invalid"};

y86_register get_Register(reg_num r)
{
    y86_register v = 0;
    if (r >= 8)
        {
            fprintf(stderr, "Illegal register: 0x%X\n", r);
            halted = TRUE;
        }
    else
        {
            char buffer[32];
            v = GP[r];
            sprintf(buffer, "%s -> 0x%08X", reg_name[r], v);
            Part2Trace(buffer);
        }
    return(v);
}

void set_Register(reg_num r, y86_register v)
{
    if (r >= 8)
        {
            fprintf(stderr, "Illegal register: 0x%X\n", r);
            halted = TRUE;
        }
    else
        {
            char buffer[32];
            sprintf(buffer, "0x%08X -> %s", v, reg_name[r]);
            Part2Trace(buffer);
            GP[r] = v;
        }
}


void set_PC(y86_address d)
{
    char buffer[32];
    sprintf(buffer, "0x%08X -> PC", d);
    Part2Trace(buffer);
    PC = d;
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void error_exit(const STRING message)
{
    fprintf(stderr, "%s.\n", message);
    exit(-1);
}



/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */
/* Load a Y86 object file */

int next_2_bytes(FILE *fp)
{
    int c1, c2;

    c1 = fgetc(fp);
    if (c1 == EOF) return(-1);
    c2 = fgetc(fp);
    if (c2 == EOF) return(-2);
    return((c1 << 8) | c2);
}
    

y86_address read_object_file(STRING filename)
{
    y86_address first_address = -1;
    y86_address load_address;

    if (debug) fprintf(stdout, "read yb file %s\n", filename);
	FILE* fp = fopen (filename, "rb");
	if (fp == NULL) 
        error_exit("The file is not a Y86 object file");
    
	/* Check the magic number */
    int magic = next_2_bytes(fp);
	if (magic != 0x7962)
        {
            if (debug) fprintf(stdout, "magic number was 0x%04X\n", magic);
            error_exit("The file is not a Y86 object file");
        }
    
	/* get the next block. */
    /* check if we have another load address */
	while ((load_address = next_2_bytes(fp)) != -1)
        {
            int size;
            
            /* Check for valid load address */
            if (load_address == -2)
                error_exit("The Y86 object file is corrupted");

            /* Remember first load address */
            if (first_address == -1) 
                first_address = load_address;
            
            /* Get size of block */
            size = next_2_bytes(fp);
            if (size < 0)
                error_exit("The Y86 object file is corrupted");

            if (debug) fprintf(stdout, "load %d bytes at 0x%08X\n", size, load_address);
            
            /* Get all data bytes and put in memory */
            while (size > 0)
                {
                    int c = fgetc(fp);
                    if (c == EOF)
                        error_exit("The Y86 object file is corrupted");
                    set_memory(load_address, c);
                    load_address += 1;
                    size -= 1;
                }
        }
    
	return(first_address);
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void scanargs(STRING s)
{
    /* check each character of the option list for
       its meaning. */

    while (*++s != '\0')
        switch (*s)
            {

            case 'd': /* debug option */
                debug = TRUE;
                break;

            case 'f': /* print flags option */
                CC_print = TRUE;
                break;

            default:
                fprintf (stderr,"y86: Bad option %c\n", *s);
                fprintf (stderr,"y86 [-d] file\n");
                exit(1);
            }
}


/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

int main (int argc, char** argv) 
{
    /* skip program name */
    argc--, argv++;

    /* check for execution flags */
    while ((argc > 0) && (**argv == '-'))
        {
            scanargs(*argv);
            argc--, argv++;
        }

    /* should be one argument left -- the file name of the program to load and execute */
	if (argc != 1)
        {
            fprintf(stderr, "usage : y86 Y86_file\n");
            return 1;
        }

    /* initialize any program data structures */
    init_memory();

    /* Read the object file into memory, and get the initial PC value */
    PC = read_object_file(argv[0]);
    if (debug) fprintf(stdout, "initial PC value = 0x%08X\n", PC);

    /* execute instructions until we halt */
    while (!halted)
        {
            /* get the opcode for the next instruction */
            int opcode = get_memory(PC);

            /* decode the instruction to get a description of it */
            struct decode_table *dte = y86_decode(opcode);
            if (debug) fprintf(stdout, "fetch and decode 0x%08X: %s\n", PC, dte->name);

            /* Update the PC by the length of the instruction */
            y86_address old_PC = PC;
            PC = PC + dte->length;

            /* execute the specific instruction. */
            switch(dte->number)
                {
                case OP_HALT: exe_halt(old_PC, dte); break;
                case OP_NOP: exe_nop(old_PC, dte); break;
                case OP_IRMOVL: exe_irmovl(old_PC, dte); break;
                case OP_RMMOVL: exe_rmmovl(old_PC, dte); break;
                case OP_MRMOVL: exe_mrmovl(old_PC, dte); break;
                case OP_CALL: exe_call(old_PC, dte); break;
                case OP_RET: exe_ret(old_PC, dte); break;
                case OP_PUSHL: exe_pushl(old_PC, dte); break;
                case OP_POPL: exe_popl(old_PC, dte); break;
                case OP_RRMOVL: exe_rrmovl(old_PC, dte); break;
                case OP_CMOVLE: exe_cmovle(old_PC, dte); break;
                case OP_CMOVL: exe_cmovl(old_PC, dte); break;
                case OP_CMOVE: exe_cmove(old_PC, dte); break;
                case OP_CMOVNE: exe_cmovne(old_PC, dte); break;
                case OP_CMOVGE: exe_cmovge(old_PC, dte); break;
                case OP_CMOVG: exe_cmovg(old_PC, dte); break;
                case OP_ADDL: exe_addl(old_PC, dte); break;
                case OP_SUBL: exe_subl(old_PC, dte); break;
                case OP_ANDL: exe_andl(old_PC, dte); break;
                case OP_XORL: exe_xorl(old_PC, dte); break;
                case OP_JMP: exe_jmp(old_PC, dte); break;
                case OP_JLE: exe_jle(old_PC, dte); break;
                case OP_JL: exe_jl(old_PC, dte); break;
                case OP_JE: exe_je(old_PC, dte); break;
                case OP_JNE: exe_jne(old_PC, dte); break;
                case OP_JGE: exe_jge(old_PC, dte); break;
                case OP_JG: exe_jg(old_PC, dte); break;
                    
                default:
                    fprintf(stdout, "0x%08X: 0x%02X -- invalid opcode\n", old_PC, opcode);
                    error_exit("invalid opcode");
                }
        }

    exit(0);
}

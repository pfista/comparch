/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

/* Memory management for Y86 */

#include "y86.h"

/* Memory is at most 32-bits of address space */
/* We simulate only those parts that we actually use,
   assuming that spatial locality will mean very little
   is actually used. */

/* we use a three level address tree */
byte **memory_level_1[1024] = {NULL};

void init_memory()
{
    int i;
    for (i = 0; i < 1024; i++) memory_level_1[i] = NULL;
}

void set_memory(y86_address a, byte value)
{
    int i;
    int top_bits = (a >> 22) & 0x3FF;
    byte **memory_level_2 = memory_level_1[top_bits];
    if (memory_level_2 == NULL)
        {
            memory_level_1[top_bits] = (byte **)malloc(1024*sizeof(byte *));
            memory_level_2 = memory_level_1[top_bits];
            if (memory_level_2 == NULL)
                error_exit("Ran out of level 2 memory");
            for (i = 0; i < 1024; i++) memory_level_2[i] = NULL;
        }
    
    int mid_bits = (a >> 12) & 0x3FF;
    byte *memory_level_3 = memory_level_2[mid_bits];
    if (memory_level_3 == NULL)
        {
            memory_level_2[mid_bits] = (byte *)calloc(4096,sizeof(byte));
            memory_level_3 = memory_level_2[mid_bits];
            if (memory_level_3 == NULL)
                error_exit("Ran out of level 3 memory");
        }

    int bottom_bits = a & 0xFFF;
    memory_level_3[bottom_bits] = value;
    if (debug) fprintf(stdout, "store 0x%02X at 0x%08X\n", value, a);
}

byte get_memory(y86_address a)
{
    byte value = 0;
    int top_bits = (a >> 22) & 0x3FF;
    byte **memory_level_2 = memory_level_1[top_bits];
    if (memory_level_2 != NULL)
        {
            int mid_bits = (a >> 12) & 0x3FF;
            byte *memory_level_3 = memory_level_2[mid_bits];
            if (memory_level_3 != NULL)
                {
                    int bottom_bits = a & 0xFFF;
                    value = memory_level_3[bottom_bits];
                }
        }
    if (debug) fprintf(stdout, "load 0x%02X from 0x%08X\n", value, a);
    return(value);

}

/* ***************************************************************** */
/*                                                                   */
/*                                                                   */
/* ***************************************************************** */

void set_memory_4(y86_address a, y86_register value, enum read_type line)
{
    int i;

    if (debug) fprintf(stdout, "store4 0x%08X at 0x%08X\n", value, a);

    char buffer[32];
    sprintf(buffer, "0x%08X -> [0x%08X]", value, a);
    Part2Trace(buffer);

    for (i = 0; i < 4; i++)
        {
            byte val = (value & 0xFF);
            set_memory(a+i, val);
            value = value >> 8;
        }
}


y86_register get_memory_4(y86_address a, enum read_type line)
{
    y86_register value = 0;
    int i;

    for (i = 3; i >= 0; i--)
        {
            byte val = get_memory(a+i);
            value = (value << 8) | val;
        }
    if (debug) fprintf(stdout, "load4 0x%08X from 0x%08X\n", value, a);

    if (line != INSTRUCTION_FETCH)
        {
            char buffer[32];
            sprintf(buffer, "[0x%08X] -> 0x%08X", a, value);
            Part2Trace(buffer);
        }
            
    return(value);
}

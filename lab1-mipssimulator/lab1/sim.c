#include <stdio.h>
#include "shell.h"
#include <stdint.h>

void process_instruction()
{

    CPU_State CURRENT_STATE, NEXT_STATE;
    CURRENT_STATE.REGS;
    CURRENT_STATE.PC;
    uint32_t jmp_address = mem_read_32(CURRENT_STATE.PC);
    uint32_t temp = CURRENT_STATE.PC || (jmp_address << 2);
    NEXT_STATE.PC = temp;







    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
}

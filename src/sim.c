#include <stdio.h>
#include "shell.h"
#include <stdint.h>
#include <stdbool.h>


// MIPS R4000 Microprocessor User's Manual -> Great Help

void process_instruction()
{
    otf ins_addiu = {
        .opcode = 0x24000000,
        .ptr = &addiu
    };
    otf ins_syscall = {
    .opcode = 0x00000000,
    .ptr = &syscall
    };

    insert_element(&ins_addiu);
    insert_element(&ins_syscall);


    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int8_t index = hash_function(curr_ins & OP_MASK);

    if (hashTable[index] != NULL && hashTable[index]->ptr != NULL) {
        void (*func)() = (void (*)())hashTable[index]->ptr; // Cast hashTable[index]->ptr and prepare it for calling later
        func();
    }
    else {
        printf("Instrucción no encontrada o puntero nulo.\n");
    }
    clean_table();
}
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */


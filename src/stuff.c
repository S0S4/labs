#include <stdio.h>
#include "shell.h"
#include <stdint.h>

// MIPS R4000 Microprocessor User's Manual -> Great Help

#define MEM_TEXT_START  0x00400000

// Instruction Formats 

// I-Type (IMMEDIATE)

#define OP_MASK 0xFC000000 // BITS 31 - 26
#define RS_MASK 0x03E00000 // BITS 25 - 21
#define RT_MASK 0x001F0000 // BITS 20 - 16
#define IM_MASK 0x0000FFFF // BITS 15 - 0

// J-Type (JUMP)

#define TA_MASK 0x03FFFFFF // BITS 25 - 0

// R-Type (REGISTER)

#define RD_MASK 0x00007C00 // BITS 15 - 11
#define SH_MASK 0x000003E0 // BITS 10 - 6
#define FU_MASK 0x0000001F // BITS 5 - 0

void process_instruction()
{

    CPU_State CURRENT_STATE, NEXT_STATE;
    CURRENT_STATE.REGS;
    CURRENT_STATE.PC = MEM_TEXT_START;

// J Instruction

// MIRAR SI LA DIRECCION DE SALTO ES CORRECTA

    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    
    printf("curr_ins: %x\n", curr_ins);
    printf("PC: \n%x\n", CURRENT_STATE.PC);
    uint32_t temp = CURRENT_STATE.PC | (curr_ins << 2);
    printf("TEMP: %x\n", temp);
    NEXT_STATE.PC = temp;
    


// JAL Instruction     

    curr_ins = mem_read_32(CURRENT_STATE.PC);
    temp = CURRENT_STATE.PC || (curr_ins << 2);
    CURRENT_STATE.REGS[31] = CURRENT_STATE.PC + 0x8;
    NEXT_STATE.PC = temp;


// BEQ Instruction 
    int i = 0;
    curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t b_target_address = (curr_ins+0x4) + (curr_ins && 0xFFFF) & 0x00000000;
    uint32_t rs = curr_ins && TA_MASK;
    for( i ; i < MIPS_REGS; i++){
         uint32_t reg_addr = mem_read_32(CURRENT_STATE.REGS[i]);
         if(reg_addr == rs){
            break;
         }
    }
    NEXT_STATE.PC = temp;


// BNE 

// BLEZ 

// BGTZ


// ADDI

    curr_ins = mem_read_32(CURRENT_STATE.PC);
    rs = curr_ins && RS_MASK;
    uint32_t rt = curr_ins && RT_MASK;
    uint16_t imm = curr_ins && IM_MASK;
    
        for(i = 0; i < MIPS_REGS; i++){
         uint32_t reg_addr = mem_read_32(CURRENT_STATE.REGS[i]);
         if(reg_addr == rs){
            uint32_t rt_aux = CURRENT_STATE.REGS[i] + imm;
            mem_write_32(rt,rt_aux);  
         }
    }

    



    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
}

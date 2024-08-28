
// MIPS R4000 Microprocessor User's Manual -> Great Help
// http://www.kurtm.net/archive/2004-Summer-cs61c-public_html/mipsasm/index.cgi -> Assembly to Hex

/* execute one instruction here. You should use CURRENT_STATE and modify
 * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
 * access memory. */


#include <stdio.h>
#include "shell.h"


void process_instruction() {

    otfn instructions[] = {
        {
            .opcode = 0x24,
                .ptr = &addiu
        },
        {
            .opcode = 0x00000000,
            .ptr = &syscall
        },
        {
            .opcode = 0x02,
            .ptr = &j
        },
        {
            .opcode = 0x03,
            .ptr = &jal
        },
        {
            .opcode = 0x04,
            .ptr = &beq
        },
        {
            .opcode = 0x05,
            .ptr = &bne
        },
        {
            .opcode = 0x06,
            .ptr = &blez
        },
        {
            .opcode = 0x07,
            .ptr = &bgtz
        },
        {
            .opcode = 0x08,
            .ptr = &addi
        },
        {
            .opcode = 0x09,
            .ptr = &addiu
        },
        {
            .opcode = 0x0A,
            .ptr = &slti
        },
        {
            .opcode = 0x0B,
            .ptr = &sltiu
        },
        {
            .opcode = 0x0C,
            .ptr = &andi
        },
        {
            .opcode = 0x0D,
            .ptr = &ori
        },
        {
            .opcode = 0x0E,
            .ptr = &xori
        },
        {
            .opcode = 0x0F,
            .ptr = &lui
        },
        {
            .opcode = 0x20,
            .ptr = &lb
        },
        {
            .opcode = 0x21,
            .ptr = &lh
        },
        {
            .opcode = 0x23,
            .ptr = &lw
        },
        {
            .opcode = 0x24,
            .ptr = &lbu
        },
        {
            .opcode = 0x25,
            .ptr = &lhu
        },
        {
            .opcode = 0x28,
            .ptr = &sb
        },
        {
            .opcode = 0x29,
            .ptr = &sh
        },
        {
            .opcode = 0x2B,
            .ptr = &sw
        },

    };

    otfs special_instructions[] = {
        {
            .opcode = 0x01,
                .inst = 0x00,
                .ptr = &bltz
        },
        {
            .opcode = 0x01,
            .inst = 0x01,
            .ptr = &bgez
        },
        {
            .opcode = 0x01,
            .inst = 0x0F,
            .ptr = &bltzal
        },
        {
            .opcode = 0x01,
            .inst = 0x11,
            .ptr = &bgezal
        },
        {
            .opcode = 0x00,
            .inst = 0x00,
            .ptr = &sll
        },
        {
            .opcode = 0x00,
            .inst = 0x02,
            .ptr = &srl
        },
        {
            .opcode = 0x00,
            .inst = 0x03,
            .ptr = &sra
        },
        {
            .opcode = 0x00,
            .inst = 0x04,
            .ptr = &sllv
        },
        {
            .opcode = 0x00,
            .inst = 0x06,
            .ptr = &srlv
        },
        {
            .opcode = 0x00,
            .inst = 0x07,
            .ptr = &srav
        },
        {
            .opcode = 0x00,
            .inst = 0x08,
            .ptr = &jr
        },
        {
            .opcode = 0x00,
            .inst = 0x09,
            .ptr = &jalr
        },
        {
            .opcode = 0x00,
            .inst = 0x20,
            .ptr = &add
        },
        {
            .opcode = 0x00,
            .inst = 0x21,
            .ptr = &addu
        },
        {
            .opcode = 0x00,
            .inst = 0x22,
            .ptr = &sub
        },
        {
            .opcode = 0x00,
            .inst = 0x23,
            .ptr = &subu
        },
        {
            .opcode = 0x00,
            .inst = 0x24,
            .ptr = &and
        },
        {
            .opcode = 0x00,
            .inst = 0x25,
            .ptr = &or
        },
        {
            .opcode = 0x00,
            .inst = 0x26,
            .ptr = & xor
        },
        {
            .opcode = 0x00,
            .inst = 0x27,
            .ptr = &nor
        },
        {
            .opcode = 0x00,
            .inst = 0x2A,
            .ptr = &slt
        },
        {
            .opcode = 0x00,
            .inst = 0x2B,
            .ptr = &sltu
        },
        {
            .opcode = 0x00,
            .inst = 0x18,
            .ptr = &mult
        },
        {
            .opcode = 0x00,
            .inst = 0x0F, 
            .ptr = &mfhi
        },
        {
            .opcode = 0x00,
            .inst = 0x12,
            .ptr = &mflo
        },
        {
            .opcode = 0x00,
            .inst = 0x11, 
            .ptr = &mthi
        },
        {
            .opcode = 0x00,
            .inst = 0x13,
            .ptr = &mtlo
        },
        {
            .opcode = 0x00,
            .inst = 0x19,
            .ptr = &multu
        },
        {
            .opcode = 0x00,
            .inst = 0x1A,
            .ptr = &div
        },
        {
            .opcode = 0x00,
            .inst = 0x1B,
            .ptr = &divu
        },
        {
            .opcode = 0x00,
            .inst = 0x0C,
            .ptr = &syscall
        }
    };

    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint8_t opcode = curr_ins & OP_MASK;
    uint8_t special_opcode = curr_ins & RT_MASK;
    uint8_t special_opcode2 = curr_ins & FU_MASK; // Instructions like MULT, DIV, MFHI identifies by the first 6 bits

    for (int i = 0; i < (sizeof(instructions) / sizeof(instructions[0])); i++) {
        if (instructions[i].opcode == opcode) {
            instructions[i].ptr();
            break;
        }
    }
    for (int i = 0; i < (sizeof(special_instructions) / sizeof(special_instructions[0])); i++) {
        if (special_instructions[i].opcode == opcode && (special_instructions[i].inst == special_opcode || special_instructions[i].inst == special_opcode2)) {
            special_instructions[i].ptr();
            break;
        }
    }

}


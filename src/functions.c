#include <stdio.h>
#include "shell.h"
#include <stdint.h>
#include <stdbool.h>




void addiu() {
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t rs = (curr_ins & RS_MASK) >> 21;
    uint32_t rt = (curr_ins & RT_MASK) >> 16;
    uint16_t imm = (curr_ins & IM_MASK);

    printf("RS: %x \nRT: %x\nIMM: %x\n", rs, rt, imm);

    uint32_t aux = imm + mem_read_32(CURRENT_STATE.REGS[rs]);
    NEXT_STATE.REGS[rt] = aux;
    NEXT_STATE.PC = CURRENT_STATE.PC + 0x4;
}
void syscall() {
    if (CURRENT_STATE.REGS[2] == 0x0A) {
        RUN_BIT = 0;
    }
}

void j() {
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t temp = CURRENT_STATE.PC | (curr_ins << 2);
    NEXT_STATE.PC = temp;
}


// JAL Instruction    

void jal() {
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t temp = CURRENT_STATE.PC || (curr_ins << 2);
    CURRENT_STATE.REGS[31] = CURRENT_STATE.PC + 0x8;
    NEXT_STATE.PC = temp;
}

// BEQ Instruction REVISAR ESTA
void beq() {
    int i = 0;
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t b_target_address = (curr_ins + 0x4) + (curr_ins && 0xFFFF) & 0x00000000;
    uint32_t rs = curr_ins && TA_MASK;
    for (i; i < MIPS_REGS; i++) {
        uint32_t reg_addr = mem_read_32(CURRENT_STATE.REGS[i]);
        if (reg_addr == rs) {
            break;
        }
    }
}

uint32_t hash_function(uint32_t key) { // FNV-1a

    uint32_t offset_basis = 2166136261;
    uint32_t fnv_prime = 16777619;
    uint32_t hash = offset_basis;

    for (uint8_t i = 1; i < 5; i++) {
        uint8_t byte = (uint8_t)((key >> (8 * i)) & 0xff);
        hash = hash ^ byte;
        hash = hash * fnv_prime;
    }
    hash = hash % tableSize;

    return hash;
}

bool insert_element(otf* ptr) {
    if (ptr == NULL) {
        return false;
    }
    int index = hash_function(ptr->opcode);
    while (hashTable[index] != NULL) {
        index++;
    }
    hashTable[index] = ptr;
    return true;
}

void clean_table() {

    int i = 0;
    while (hashTable[i] != NULL) {
        hashTable[i]->opcode = NULL;
        hashTable[i]->ptr = NULL;
        hashTable[i] = NULL;
        i++;
    }

}
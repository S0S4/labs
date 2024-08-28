#include <stdio.h>
#include "shell.h"
#include <stdint.h>
#include <stdbool.h>

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


/* TO DO
REVISAR LAS INSTRUCCIONES
HACER HASHMAP CON TODAS LAS INSTRUCCIONES

*/

void addiu() { // ADD Unsigned Immediate 
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int8_t rs = (curr_ins & RS_MASK) >> 21;
    int8_t rt = (curr_ins & RT_MASK) >> 16;
    uint16_t imm = (curr_ins & IM_MASK);

    printf("RS: %x \nRT: %x\nIMM: %x\n", rs, rt, imm);

    uint32_t aux = imm + mem_read_32(CURRENT_STATE.REGS[rs]);
    NEXT_STATE.REGS[rt] = aux;
    NEXT_STATE.PC = CURRENT_STATE.PC + 0x4;
}
void syscall() { // SYSCALL
    if (CURRENT_STATE.REGS[2] == 0x0A) {
        RUN_BIT = 0;
    }
}
void j() { // Jump
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t temp = CURRENT_STATE.PC | (curr_ins << 2);
    NEXT_STATE.PC = temp;
}

// JAL Instruction    

void jal() { // Jump And Link
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t temp = CURRENT_STATE.PC | (curr_ins << 2);
    CURRENT_STATE.REGS[31] = CURRENT_STATE.PC + 0x8;
    NEXT_STATE.PC = temp;
}
void beq() { // Branch On Equal
    int i = 0;
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t offset = (curr_ins & 0xFFFF) << 2;
    uint32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t b_target_address = (curr_ins + 0x4) + offset;
    int8_t rs = curr_ins && RS_MASK;
    int8_t rt = curr_ins && RT_MASK;

    if (mem_read_32(CURRENT_STATE.REGS[rs]) == mem_read_32(CURRENT_STATE.REGS[rt])) {
        NEXT_STATE.PC = b_target_address;
    }
}

void bne() { // Branch On Not Equal
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t addr = (int32_t)((curr_ins + 4) << 2);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    if (rs != rt) {
        NEXT_STATE.PC = addr;
    }
}

void blez() { // Branch on Less Than Or Equal To Zero

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t addr = (int32_t)((curr_ins + 4) << 2);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);

    if (rs <= 0) {
        NEXT_STATE.PC = addr;
    }
}

void bgtz() { // Branch On Greater Than Zero
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t addr = (int32_t)((curr_ins + 4) << 2);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);

    if (rt > 0) {
        NEXT_STATE.PC = addr;
    }

}

void addi() { // ADD Immediate

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);

    int32_t sum = imm + rs;

    if ((sum & 0x80000000) != (sum & 0x40000000)) { // Detect overflow
        NEXT_STATE.REGS[rt] = -1;
        puts("Overflow!");
    }
    else {

        NEXT_STATE.REGS[rt] = imm + mem_read_32(CURRENT_STATE.REGS[rs]);
    }

}


void slti() { // Set On Less Than Immediate
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);

    if (rs_val < imm) {
        NEXT_STATE.REGS[rt] = 1;
    }
    else {
        NEXT_STATE.REGS[rt] = 0;
    }
}

void sltiu() { // Set On Less Than Immediate Unsigned
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    uint32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    if (rs_val < imm) {
        NEXT_STATE.REGS[rt] = 1;
    }
    else {
        NEXT_STATE.REGS[rt] = 0;
    }
}

void andi() { // AND Immediate
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);

    int32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    NEXT_STATE.REGS[rt] = rs_val & imm;

}

void ori() { // OR Immediate

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);

    int32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    NEXT_STATE.REGS[rt] = rs_val | imm;

}

void xori() { // Exclusive OR Immediate

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);

    int32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    NEXT_STATE.REGS[rt] = rs_val ^ imm;
}

void lui() { // Load Upper Immediate 
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK) << 16;
    int8_t rt = (curr_ins & RT_MASK);

    NEXT_STATE.REGS[rt] = imm;
}

void lb() { // Load Byte
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    if (virt_addr & 0x1 != 0) {
        puts("!Exception");
        exit(1);
    }

    int32_t virt_addr_val = (int32_t)(mem_read_32(virt_addr) && 0xFF); // Take the byte and sign-extend it
    NEXT_STATE.REGS[rt] = virt_addr_val;
}

void lbu() { // Load Unsigned Byte
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    if (virt_addr & 0x1 != 0) {
        puts("!Exception");
        exit(1);
    }

    uint32_t virt_addr_val = (uint32_t)(mem_read_32(virt_addr) && 0xFF); // Take the byte and sign-extend it
    NEXT_STATE.REGS[rt] = virt_addr_val;
}

void lh() { // Load HalfWord
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    if (virt_addr & 0x1 != 0) {
        puts("!Exception");
        exit(1);
    }

    int32_t virt_addr_val = (int32_t)(mem_read_32(virt_addr) && 0xFFFF);
    NEXT_STATE.REGS[rt] = virt_addr_val;
}

void lhu() { // Load Unsigned HalfWord
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    uint32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    if (virt_addr & 0x1 != 0) {
        puts("!Exception");
        exit(1);
    }

    uint32_t virt_addr_val = (uint32_t)(mem_read_32(virt_addr) && 0xFFFF);
    NEXT_STATE.REGS[rt] = virt_addr_val;
}

void lw() { // Load Word
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    if (virt_addr & 0x1 != 0) {
        puts("!Exception");
        exit(1);
    }

    int32_t virt_addr_val = (int32_t)(mem_read_32(virt_addr) && 0xFFFFFFFF);
    NEXT_STATE.REGS[rt] = virt_addr_val;
}

void sb() { // Store Byte
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    int8_t lsb_rt = mem_read_32(CURRENT_STATE.REGS[rt] & 0xFF);
    mem_write_32(virt_addr, lsb_rt);
}

void sh() { // Store Halfword
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    int16_t lsb_rt = mem_read_32(CURRENT_STATE.REGS[rt] & 0xFFFF);
    mem_write_32(virt_addr, lsb_rt);
}

void sw() { // Store Word
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t imm = (int32_t)(curr_ins & IM_MASK);
    int8_t base = (curr_ins & RS_MASK);
    int8_t rt = (curr_ins & RT_MASK);
    int32_t base_val = mem_read_32(CURRENT_STATE.REGS[base]);
    int32_t virt_addr = imm + base_val;

    int32_t lsb_rt = mem_read_32(CURRENT_STATE.REGS[rt] & 0xFFFFFFFF);
    mem_write_32(virt_addr, lsb_rt);
}

void bltz() { // Branch On Less Than Zero
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t offset = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int32_t next_ins = curr_ins + 4;

    if (mem_read_32(CURRENT_STATE.REGS[rs] & 0x80000000)) {
        NEXT_STATE.PC = next_ins + (offset << 2);
    }
}
void bgez() { // Branch On Greater Than Or Equal to Zero

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t offset = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int32_t next_ins = curr_ins + 4;

    if (!(mem_read_32(CURRENT_STATE.REGS[rs] & 0x80000000))) {
        NEXT_STATE.PC = next_ins + (offset << 2);
    }
}

void bltzal() { // Branch On Less Than Zero And Link
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t offset = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int32_t next_ins = curr_ins + 4;
    int32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);


    NEXT_STATE.REGS[31] = next_ins + 4;

    if (mem_read_32(CURRENT_STATE.REGS[rs] & 0x80000000)) {
        NEXT_STATE.PC = rs_val + offset;
    }
}

void bgezal() { // 
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t offset = (int32_t)(curr_ins & IM_MASK);
    int8_t rs = (curr_ins & RS_MASK);
    int32_t next_ins = curr_ins + 4;
    int32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);

    NEXT_STATE.REGS[31] = next_ins + 4;

    if (mem_read_32(CURRENT_STATE.REGS[rs] & 0x80000000 == 0)) {
        NEXT_STATE.PC = rs_val + offset;
    }
}

void sll() { // Shift Left Logical

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    int32_t sa = curr_ins & 0x1E0;
    int32_t sll = 0x0;



    NEXT_STATE.REGS[rd] = (uint32_t)NEXT_STATE.REGS[rt] << (uint32_t)sa;
}

void srl() { // Shift Right Logical
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    int32_t sa = curr_ins & 0x1E0;
    int32_t sll = 0x0;

    NEXT_STATE.REGS[rd] = (uint32_t)NEXT_STATE.REGS[rt] >> (uint32_t)sa;
}


void sra() { // Shift Right Arithmetic
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    int32_t sa = curr_ins & 0x1E0;
    int32_t sll = 0x0;

    NEXT_STATE.REGS[rd] = NEXT_STATE.REGS[rt] >> sa;
}

void sllv() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    uint32_t valrs = (mem_read_32(CURRENT_STATE.REGS[rs] & 0x1F));

    NEXT_STATE.REGS[rd] = (uint32_t)NEXT_STATE.REGS[rt] << valrs;
}


void srlv() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    uint32_t valrs = (mem_read_32(CURRENT_STATE.REGS[rs] & 0x1F));

    NEXT_STATE.REGS[rd] = (uint32_t)NEXT_STATE.REGS[rt] >> valrs;
}

void srav() {

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    uint32_t valrs = (mem_read_32(CURRENT_STATE.REGS[rs]));

    NEXT_STATE.REGS[rd] = (uint32_t)NEXT_STATE.REGS[rt] >> valrs;

}

void jr() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rs = curr_ins & RS_MASK;

    if ((rs & 0x3) == 0) {
        NEXT_STATE.PC = mem_read_32(CURRENT_STATE.REGS[rs]);
    }
    else {
        puts("Address Exception");
        exit(1);
    }
}

void jalr() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rs = curr_ins & RS_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = curr_ins + 8; // Address after the delay slot

    if ((rs & 0x3) == 0) {
        NEXT_STATE.PC = mem_read_32(CURRENT_STATE.REGS[rs]);
    }
    else {
        puts("Address Exception");
        exit(1);
    }


}

void add() {

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = mem_read_32(CURRENT_STATE.REGS[rs]) | mem_read_32(CURRENT_STATE.REGS[rt]);

}

void addu() {

    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = (uint32_t)(mem_read_32(CURRENT_STATE.REGS[rs]) + mem_read_32(CURRENT_STATE.REGS[rt]));

}

void sub() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = mem_read_32(CURRENT_STATE.REGS[rs]) - mem_read_32(CURRENT_STATE.REGS[rt]);

}

void subu() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = (uint32_t)(mem_read_32(CURRENT_STATE.REGS[rs]) | mem_read_32(CURRENT_STATE.REGS[rt]));
}

void and () {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = mem_read_32(CURRENT_STATE.REGS[rs]) & mem_read_32(CURRENT_STATE.REGS[rt]);
}
void or () {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = mem_read_32(CURRENT_STATE.REGS[rs]) | mem_read_32(CURRENT_STATE.REGS[rt]);
}

void xor () {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = (uint32_t)(mem_read_32(CURRENT_STATE.REGS[rs]) ^ mem_read_32(CURRENT_STATE.REGS[rt]));
}

void nor() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;

    NEXT_STATE.REGS[rd] = !(mem_read_32(CURRENT_STATE.REGS[rs]) | mem_read_32(CURRENT_STATE.REGS[rt]));
}

void slt() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    if (mem_read_32(rs) < mem_read_32(rt)) {
        NEXT_STATE.REGS[rd] = 1;

    }
    else {
        NEXT_STATE.REGS[rd] = 0;
    }
}

void sltu() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    uint32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    uint32_t rt_val = mem_read_32(CURRENT_STATE.REGS[rt]);
    if (rs_val < rt_val) {
        NEXT_STATE.REGS[rd] = 1;

    }
    else {
        NEXT_STATE.REGS[rd] = 0;
    }
}
void mult() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    uint32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    uint32_t rt_val = mem_read_32(CURRENT_STATE.REGS[rt]);

    NEXT_STATE.LO = (rs_val * rt_val) & 0xFFFF;
    NEXT_STATE.HI = (rs_val * rt_val) & 0xFFFF0000;
}
void mfhi() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rd = curr_ins & RD_MASK;
    NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
}
void mflo() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rd = curr_ins & RD_MASK;
    NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
}
void mthi() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rs = curr_ins & RS_MASK;
    NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
}
void mtlo() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);
    int32_t rs = curr_ins & RS_MASK;
    NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
}
void multu() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    uint32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    uint32_t rt_val = mem_read_32(CURRENT_STATE.REGS[rt]);

    NEXT_STATE.LO = (uint32_t)(rs_val * rt_val) & 0xFFFF;
    NEXT_STATE.HI = (uint32_t)(rs_val * rt_val) & 0xFFFF0000;
}
void div() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    uint32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    uint32_t rt_val = mem_read_32(CURRENT_STATE.REGS[rt]);

    if (rt_val == 0) {
        exit(1);
    }

    NEXT_STATE.LO = (rs_val / rt_val) & 0xFFFF;
    NEXT_STATE.HI = (rs_val % rt_val) & 0xFFFF0000;
}
void divu() {
    int32_t curr_ins = mem_read_32(CURRENT_STATE.PC);

    int32_t rs = curr_ins & RS_MASK;
    int32_t rt = curr_ins & RT_MASK;
    int32_t rd = curr_ins & RD_MASK;
    uint32_t rs_val = mem_read_32(CURRENT_STATE.REGS[rs]);
    uint32_t rt_val = mem_read_32(CURRENT_STATE.REGS[rt]);

    if (rt_val == 0) {
        exit(1);
    }
    NEXT_STATE.LO = (uint32_t)(rs_val / rt_val) & 0xFFFF;
    NEXT_STATE.HI = (uint32_t)(rs_val % rt_val) & 0xFFFF0000;
}

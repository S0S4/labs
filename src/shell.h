/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   ECE 447                                                   */
/*   Carnegie Mellon University                                */
/*                                                             */
/***************************************************************/

#define _SIM_SHELL_H_
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

#define tableSize 56
#include <stdint.h>

#define FALSE 0
#define TRUE  1

#define MIPS_REGS 32
#define tableSize 56

typedef struct CPU_State_Struct {

  uint32_t PC;		/* program counter */
  uint32_t REGS[MIPS_REGS]; /* register file. */
  uint32_t HI, LO;          /* special regs for mult/div. */
} CPU_State;

/* Data Structure for Latch */

extern CPU_State CURRENT_STATE, NEXT_STATE;

extern int RUN_BIT;	/* run bit */

uint32_t mem_read_32(uint32_t address);
void     mem_write_32(uint32_t address, uint32_t value);

/* YOU IMPLEMENT THIS FUNCTION */
void process_instruction();

typedef struct opcodeToFunction {
    uint32_t opcode;
    void* ptr;
}	otf;

extern otf* hashTable[tableSize];

void addiu();
void j();
void jal();
void beq();
void syscall();


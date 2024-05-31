//
// Created by niclassic on 08.05.24.
//
#ifndef INSTRUCTION_TYPES_H
#define INSTRUCTION_TYPES_H

#include <stdint.h>

typedef struct rtype {
    uint8_t opcode;
    uint8_t rd;
    uint8_t funct3;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t funct7;
} rtype;

typedef struct itype {
    uint8_t opcode;
    uint8_t rd;
    uint8_t funct3;
    uint8_t rs1;
    int16_t imm;
} itype;

typedef struct stype {
    uint8_t opcode;
    int16_t imm;
    uint8_t funct3;
    uint8_t rs1;
    uint8_t rs2;
} stype;

typedef struct btype {
    uint8_t opcode;
    int16_t imm;
    uint8_t funct3;
    uint8_t rs1;
    uint8_t rs2;
} btype;

typedef struct utype {
    uint8_t opcode;
    uint8_t rd;
    int32_t imm;
} utype;

typedef struct jtype {
    uint8_t opcode;
    uint8_t rd;
    int32_t imm;
} jtype;

typedef enum {
    R_TYPE, //Register
    I_TYPE, //Immediate
    S_TYPE, //Store
    B_TYPE, //Branch
    U_TYPE, //Upper immediate
    J_TYPE, //Jump
    FENCE, //Fence
    INVALID, //Unknown
} instruction_type;

typedef struct {
    instruction_type type;
    union {
        rtype r_intr;
        itype i_instr;
        stype s_instr;
        btype b_instr;
        utype u_instr;
        jtype j_instr;
    } instr;
} instruction;

instruction decode_instruction(uint32_t number);

const char *to_string(instruction instruction);

#endif

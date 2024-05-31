//
// Created by niclassic on 08.05.24.
//

#include <stdlib.h>
#include <stdio.h>
#include "instruction_types.h"

#define INVALID_MSG "Invalid instruction"

/**
 * Extracts bits from a given number to an uint8_t
 *
 * @param number The number bits should be extracted from
 * @param start  The offset of the first bit where extraction should start (INCLUSIVE)
 * @param length The number of bits to be extracted
 * @return The extracted bits
 */
static inline uint8_t extract_bits_to_char(uint32_t number, int32_t start, int32_t length) {
    uint32_t mask = (1 << length) - 1;
    return (uint8_t) ((number >> start) & mask);
}

/**
 * Extracts the opcode from an instruction
 *
 * @param number The instruction
 * @return The opcode
 */
static inline uint8_t extract_opcode(const uint32_t number) {
    return extract_bits_to_char(number, 0, 7);
}

/**
 * Extracts the destination register from an instruction
 *
 * @param number The instruction
 * @return The destination register
 */
static inline uint8_t extract_rd(const uint32_t number) {
    return extract_bits_to_char(number, 7, 5);
}


/**
 * Extracts the funct3 from an instruction
 *
 * @param number The instruction
 * @return The funct3-field
 */
static inline uint8_t extract_funct3(const uint32_t number) {
    return extract_bits_to_char(number, 12, 3);
}


/**
 * Extracts the first source register from an instruction
 *
 * @param number The instruction
 * @return The first source register
 */
static inline uint8_t extract_rs1(const uint32_t number) {
    return extract_bits_to_char(number, 15, 5);
}


/**
 * Extracts the second source register from an instruction
 *
 * @param number The instruction
 * @return The second source register
 */
static inline uint8_t extract_rs2(const uint32_t number) {
    return extract_bits_to_char(number, 20, 5);
}

rtype parse_rtype(const uint32_t number) {
    rtype instruction;
    instruction.opcode = extract_opcode(number);
    instruction.rd = extract_rd(number);
    instruction.funct3 = extract_funct3(number);
    instruction.rs1 = extract_rs1(number);
    instruction.rs2 = extract_rs2(number);
    instruction.funct7 = extract_bits_to_char(number, 25, 7);
    return instruction;
}

itype parse_itype(const uint32_t number) {
    itype instruction;
    instruction.opcode = extract_opcode(number);
    instruction.rd = extract_rd(number);
    instruction.funct3 = extract_funct3(number);
    instruction.rs1 = extract_rs1(number);
    instruction.imm = (int16_t) ((int32_t) number >> 20);
    return instruction;
}

stype parse_stype(const uint32_t number) {
    stype instruction;
    instruction.opcode = extract_opcode(number);
    instruction.funct3 = extract_funct3(number);
    instruction.rs1 = extract_rs1(number);
    instruction.rs2 = extract_rs2(number);

    uint16_t imm11_5 = (number >> 25) & 0x7F; // Bits 11-5
    uint16_t imm4_0 = (number >> 7) & 0x1F; // Bits 4-0

    instruction.imm = (int16_t) ((imm11_5 << 5) | imm4_0);
    //Vorzeichen prüfen => Sign extension
    if (imm11_5 >> 6) {
        instruction.imm |= (int16_t) 0xF000; // Oberen 4 bit für Vorzeichen auffüllen
    }
    return instruction;
}

btype parse_btype(const uint32_t number) {
    btype instruction;
    instruction.opcode = extract_opcode(number);
    instruction.funct3 = extract_funct3(number);
    instruction.rs1 = extract_rs1(number);
    instruction.rs2 = extract_rs2(number);

    uint16_t imm11 = (number >> 7) & 0x01; // Bit 11
    uint16_t imm4_1 = (number >> 8) & 0x0F; // Bits 4-1
    uint16_t imm10_5 = (number >> 25) & 0x3F; // Bits 10-5
    uint16_t imm12 = (number >> 31) & 0x01; // Bit 12

    instruction.imm = (int16_t) ((imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1) | (imm12 << 12));
    //Vorzeichen prüfen => Sign extension
    if (imm12) {
        instruction.imm |= (int16_t) 0xF000; // Oberen 4 bit für Vorzeichen auffüllen
    }
    return instruction;
}

utype parse_utype(const uint32_t number) {
    utype instruction;
    instruction.opcode = extract_opcode(number);
    instruction.rd = extract_rd(number);
    instruction.imm = ((int32_t) (number) >> 12);
    return instruction;
}

jtype parse_jtype(const uint32_t number) {
    jtype instruction;
    instruction.opcode = extract_opcode(number);
    instruction.rd = extract_rd(number);

    uint16_t imm20 = (number >> 31) & 0x01; // Bit 20
    uint16_t imm10_1 = (number >> 21) & 0x3FF; //Bits 10-1
    uint16_t imm11 = (number >> 20) & 0x01; // Bit 11
    uint16_t imm19_12 = (number >> 12) & 0xFF; // Bits 19-12

    instruction.imm = (int16_t) ((imm20 << 20) | (imm10_1 << 1) | (imm11 << 11) | (imm19_12 << 12));
    //Vorzeichen prüfen => Sign extension
    if (imm20) {
        instruction.imm |= (int32_t) 0xFF000000;
    }
    return instruction;
}


instruction decode_instruction(const uint32_t number) {
    instruction decoded;
    uint8_t opcode = number & 0b1111111;
    switch (opcode) {
        case 0b0110111:
        case 0b0010111:
            decoded.type = U_TYPE;
            decoded.instr.u_instr = parse_utype(number);
            break;
        case 0b1101111:
            decoded.type = J_TYPE;
            decoded.instr.j_instr = parse_jtype(number);
            break;
        case 0b1100111:
        case 0b0000011:
        case 0b0010011:
        case 0b1110011:
            decoded.type = I_TYPE;
            decoded.instr.i_instr = parse_itype(number);
            break;
        case 0b1100011:
            decoded.type = B_TYPE;
            decoded.instr.b_instr = parse_btype(number);
            break;
        case 0b0100011:
            decoded.type = S_TYPE;
            decoded.instr.s_instr = parse_stype(number);
            break;
        case 0b0110011:
            decoded.type = R_TYPE;
            decoded.instr.r_intr = parse_rtype(number);
            break;
        case 0b0001111:
            decoded.type = FENCE;
            break;
        default:
            decoded.type = INVALID;
    }
    return decoded;
}

void rtype_to_instruction_string(char *str, rtype instr, const char *cmd) {
    sprintf(str, "%s x%d x%d x%d", cmd, instr.rd, instr.rs1, instr.rs2);
}

void itype_to_load_instruction_string(char *str, itype instr, const char *cmd) {
    sprintf(str, "%s x%d %d(x%d)", cmd, instr.rd, instr.imm, instr.rs1);
}

void itype_to_instruction_string(char *str, itype instr, const char *cmd) {
    sprintf(str, "%s x%d x%d %d", cmd, instr.rd, instr.rs1, instr.imm);
}

void itype_shift_to_instruction_string(char *str, itype instr, const char *cmd) {
    sprintf(str, "%s x%d x%d %d", cmd, instr.rd, instr.rs1, instr.imm & 0x1F);
}

void stype_to_instruction_string(char *str, stype instr, const char *cmd) {
    sprintf(str, "%s x%d %d(x%d)", cmd, instr.rs2, instr.imm, instr.rs1);
}

void btype_to_instruction_string(char *str, btype instr, const char *cmd) {
    sprintf(str, "%s x%d x%d %d", cmd, instr.rs1, instr.rs2, instr.imm);
}

void jtype_to_instruction_string(char *str, jtype instr, const char *cmd) {
    sprintf(str, "%s x%d %d", cmd, instr.rd, instr.imm);
}

void utype_to_instruction_string(char *str, utype instr, const char *cmd) {
    sprintf(str, "%s x%d %d", cmd, instr.rd, instr.imm);
}

const char *rtype_to_string(const instruction *instruction, char *str) {
    rtype instr = (*instruction).instr.r_intr;
    switch (instr.funct3) {
        case 0b000:
            if (instr.funct7 == 0) {
                rtype_to_instruction_string(str, instr, "ADD");
            } else if (instr.funct7 == (0b1 << 5)) {
                rtype_to_instruction_string(str, instr, "SUB");
            }
            break;
        case 0b001:
            rtype_to_instruction_string(str, instr, "SLL");
            break;
        case 0b010:
            rtype_to_instruction_string(str, instr, "SLT");
            break;
        case 0b011:
            rtype_to_instruction_string(str, instr, "SLTU");
            break;
        case 0b100:
            rtype_to_instruction_string(str, instr, "XOR");
            break;
        case 0b101:
            if (instr.funct7 == 0) {
                rtype_to_instruction_string(str, instr, "SRL");
            } else if (instr.funct7 == (0b1 << 5)) {
                rtype_to_instruction_string(str, instr, "SRA");
            }
            break;
        case 0b110:
            rtype_to_instruction_string(str, instr, "OR");
            break;
        case 0b111:
            rtype_to_instruction_string(str, instr, "AND");
            break;
    }
    return INVALID_MSG;
}

const char *itype_to_string(const instruction *instruction, char *str) {
    itype instr = (*instruction).instr.i_instr;
    if (instr.opcode == 0b1100111) {
        itype_to_instruction_string(str, instr, "JALR");
    } else if (instr.opcode == 0b0000011) {
        switch (instr.funct3) {
            case 0b000:
                itype_to_load_instruction_string(str, instr, "LB");
                break;
            case 0b001:
                itype_to_load_instruction_string(str, instr, "LH");
                break;
            case 0b010:
                itype_to_load_instruction_string(str, instr, "LW");
                break;
            case 0b100:
                itype_to_load_instruction_string(str, instr, "LBU");
                break;
            case 0b101:
                itype_to_load_instruction_string(str, instr, "LHU");
                break;
        }
    } else if (instr.opcode == 0b0010011) {
        switch (instr.funct3) {
            case 0b000:
                itype_to_instruction_string(str, instr, "ADDI");
                break;
            case 0b010:
                itype_to_instruction_string(str, instr, "SLTI");
                break;
            case 0b011:
                itype_to_instruction_string(str, instr, "SLTU");
                break;
            case 0b100:
                itype_to_instruction_string(str, instr, "XORI");
                break;
            case 0b110:
                itype_to_instruction_string(str, instr, "ORI");
                break;
            case 0b111:
                itype_to_instruction_string(str, instr, "ANDI");
                break;
            case 0b001:
                itype_shift_to_instruction_string(str, instr, "SLLI");
                break;
            case 0b101:
                if (instr.imm == 0) {
                    itype_shift_to_instruction_string(str, instr, "SRLI");
                } else if (instr.imm == (0b1 << 5)) {
                    itype_shift_to_instruction_string(str, instr, "SRAI");
                }
                break;
        }
    } else if (instr.opcode == 0b1110011) {
        if (instr.imm == 0) {
            sprintf(str, "ECALL");
        } else if (instr.imm == 1) {
            sprintf(str, "EBREAK");
        }
    }
    return INVALID_MSG;
}

const char *stype_to_string(const instruction *instruction, char *str) {
    stype instr = (*instruction).instr.s_instr;
    switch (instr.funct3) {
        case 0b000:
            stype_to_instruction_string(str, instr, "SB");
            break;
        case 0b001:
            stype_to_instruction_string(str, instr, "SH");
            break;
        case 0b010:
            stype_to_instruction_string(str, instr, "SW");
            break;
    }
    return INVALID_MSG;
}

const char *btype_to_string(const instruction *instruction, char *str) {
    btype instr = (*instruction).instr.b_instr;
    switch (instr.funct3) {
        case 0b000:
            btype_to_instruction_string(str, instr, "BEQ");
            break;
        case 0b001:
            btype_to_instruction_string(str, instr, "BNE");
            break;
        case 0b100:
            btype_to_instruction_string(str, instr, "BLT");
            break;
        case 0b101:
            btype_to_instruction_string(str, instr, "BGE");
            break;
        case 0b110:
            btype_to_instruction_string(str, instr, "BLTU");
            break;
        case 0b111:
            btype_to_instruction_string(str, instr, "BGEU");
            break;
    }
    return INVALID_MSG;
}

const char *utype_to_string(const instruction *instruction, char *str) {
    utype instr = (*instruction).instr.u_instr;
    if (instr.opcode == 0b0110111) {
        utype_to_instruction_string(str, instr, "LUI");
    } else if (instr.opcode == 0b0010111) {
        utype_to_instruction_string(str, instr, "AUIPC");
    }
    return INVALID_MSG;
}

const char *jtype_to_string(const instruction *instruction, char *str) {
    jtype instr = (*instruction).instr.j_instr;
    if (instr.opcode == 0b1101111) {
        jtype_to_instruction_string(str, instr, "JAL");
    }
    return INVALID_MSG;
}

const char *to_string(const instruction instruction) {
    char *str = (char *) malloc(64 * sizeof(char));
    if (str == NULL) {
        return NULL;
    }
    switch (instruction.type) {
        case R_TYPE:
            rtype_to_string(&instruction, str);
            break;
        case I_TYPE:
            itype_to_string(&instruction, str);
            break;
        case S_TYPE:
            stype_to_string(&instruction, str);
            break;
        case B_TYPE:
            btype_to_string(&instruction, str);
            break;
        case U_TYPE:
            utype_to_string(&instruction, str);
            break;
        case J_TYPE:
            jtype_to_string(&instruction, str);
            break;
        case FENCE:
            sprintf(str, "FENCE");
            break;
        case INVALID:
            sprintf(str, INVALID_MSG);
            break;
    }
    return str;
}

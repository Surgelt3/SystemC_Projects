#ifndef DEFS_H
#define DEFS_H

enum ALUOps {
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_SLT,
    ALU_SLL,
    ALU_SRL,
    ALU_SRA,
    ALU_NOP
};

enum Ops {
    ADD,
    SUB,
    AND,
    OR,
    XOR,
    SLT,
    SLL,
    SRL,
    SRA,
    NOP,
    ADDI,
    ANDI,
    ORI,
    XORI,
    SLTI,
    SLLI,
    SRLI,
    SRAI,
    LW,
    SW,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    JAL,
    JALR
};

#endif

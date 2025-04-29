#ifndef CNTRL_U_H
#define CNTRL_U_H

#include <systemc>
#include "defs.h"

SC_MODULE(CNTRL_U){

    sc_in<sc_uint<7>> op;
    sc_in<sc_uint<3>> funct3;
    sc_in<sc_uint<7>> funct7;
    sc_out<ALUOps> ALUop;

    SC_CTOR(CNTRL_U){
        
    }

    

    Ops decode_instr(sc_uint<7> op, sc_uint<3> funct3, sc_uint<7> funct7) {
        switch (op){
            case 0b0110011: 
                switch(funct3){
                    case 0b000: return funct7[5] ? SUB:ADD;
                    case 0b001: return SLL;
                    case 0b010: return SLT;
                    case 0b100: return XOR;
                    case 0b101: return funct7[5] ? SRA:SRL;
                    case 0b110: return OR;
                    case 0b111: return AND;
                }
            case 0b0010011:
                switch(funct3){
                    case 0b000: return ADDI;
                    case 0b001: return SLLI;
                    case 0b010: return SLTI;
                    case 0b100: return XORI;
                    case 0b101: return funct7[5] ? SRAI:SRLI;
                    case 0b110: return ORI;
                    case 0b111: return ANDI;
                }
            case 0b0000011:
                switch (funct3){
                    case 0b010: return LW;
                }
            case 0b0100011:
                switch (funct3){
                    case 0b010: return SW;
                }
            case 0b1100011:
                switch(funct3){
                    case 0b000: return BEQ;
                    case 0b001: return BNE;
                    case 0b100: return BLT;
                    case 0b101: return BGE;
                    case 0b110: return BLTU;
                    case 0b111: return BGEU;
                }
            case 0b1101111:
                switch(funct3){
                    case 0b000: return JALR;
                    default: return JAL;
                }
            
        } 
    }

};


#endif
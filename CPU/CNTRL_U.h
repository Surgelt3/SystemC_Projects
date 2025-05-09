#ifndef CNTRL_U_H
#define CNTRL_U_H

#include <systemc>
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;


SC_MODULE(CNTRL_U){

    sc_in<sc_uint<32>> instr;
    sc_out<sc_int<32>> ext_out;
    sc_out<Ops> ALUop;

    SC_CTOR(CNTRL_U){
        SC_METHOD(call_func);
        sensitive << instr;
    }

    void call_func(){
        ALUop.write(decode_instr());
    }



    Ops decode_instr() {
        sc_uint<32> instruction = instr.read();
        sc_uint<7> op = instruction.range(6, 0);
        sc_uint<3> funct3 = instruction.range(14, 12);
        sc_uint<1> funct7b5 = instruction.range(30, 30);

        switch (op){
            case 0b0110011:
                ext_out.write(0);
                switch(funct3){
                    case 0b000: return funct7b5 ? SUB:ADD;
                    case 0b001: return SLL;
                    case 0b010: return SLT;
                    case 0b100: return XOR;
                    case 0b101: return funct7b5 ? SRA:SRL;
                    case 0b110: return OR;
                    case 0b111: return AND;
                }
            case 0b0010011:
                ext_out.write((sc_int<32>) (sc_int<12>) sc_bv<12>(instruction.range(31, 20)));
                switch(funct3){
                    case 0b000: return ADDI;
                    case 0b001: return SLLI;
                    case 0b010: return SLTI;
                    case 0b100: return XORI;
                    case 0b101: return funct7b5 ? SRAI:SRLI;
                    case 0b110: return ORI;
                    case 0b111: return ANDI;
                }
            case 0b0000011:
                ext_out.write((sc_int<32>) (sc_int<12>) sc_bv<12>(instruction.range(31, 20)));
                switch (funct3){
                    case 0b010: return LW;
                }
            case 0b0100011:
                ext_out.write(
                    (sc_int<32>) (sc_int<12>) 
                    (instruction.range(31,25), instruction.range(11,7))
                );
                switch (funct3){
                    case 0b010: return SW;
                }
            case 0b1100011:
                ext_out.write(
                    (sc_int<32>) (sc_int<13>) sc_bv<13>
                    ((instruction[31], instruction[7], instruction.range(30,25), instruction.range(11,8), sc_bv<1>("0")))
                );
                switch(funct3){
                    case 0b000: return BEQ;
                    case 0b001: return BNE;
                    case 0b100: return BLT;
                    case 0b101: return BGE;
                    case 0b110: return BLTU;
                    case 0b111: return BGEU;
                }
            case 0b1101111: {
                sc_uint<20> payload = (instruction[31], 
                    instruction.range(19,12), 
                    instruction[20], 
                    instruction.range(30,21));
                sc_int<32> imm = (sc_int<32>)( (sc_int<21>)payload << 1 );
                ext_out.write(imm);                
                return JAL; 
            }
            case 0b1100111:
                ext_out.write((sc_int<32>) (sc_int<12>) sc_bv<12>(instruction.range(31, 20)));
                switch(funct3){
                    case 0b000: return JALR;
                }
            default: ext_out.write(0); return NOP;
        } 
    }

};


#endif
#ifndef EX_H
#define EX_H

#include <systemc>
#include "defs.h"
#include "ALU.h"


using namespace sc_core;
using namespace sc_dt;


SC_MODULE(EX){

    sc_in<sc_int<32>> RD1, RD2, Imm;
    sc_in<sc_uint<32>> PC;
    sc_in<Ops> op;

    sc_out<bool> Zero;
    sc_out<sc_int<32>> ALUResult;
    sc_out<sc_uint<32>> PCTarget;
    sc_signal<sc_int<32>> alu_in2;
    

    ALU* alu;

    SC_CTOR(EX){

        SC_METHOD(select_alu_input);
        sensitive << alu_in2 << RD2 << op;

        alu = new ALU("Alu");
        alu->in1(RD1);
        alu->in2(alu_in2);
        alu->op(op);
        alu->out(ALUResult);
        alu->zero(Zero);

        SC_METHOD(PCPlus);
        sensitive << Imm << PC;
    }

    bool imm_op(Ops op){
        switch (op) {
            case ADDI:
            case ANDI:
            case ORI:
            case XORI:
            case SLTI:
            case SLLI:
            case SRLI:
            case SRAI:
            case LW:
            case SW:
            case JALR:
                return true;
            default:
                return false;
        }
    }

    void select_alu_input(){
        if (imm_op(op.read())) {
            alu_in2.write(Imm.read());
        } else {
            alu_in2.write(RD2.read());
        }
    }

    void PCPlus(){
        PCTarget.write(Imm.read() + PC.read());
    }


};

#endif
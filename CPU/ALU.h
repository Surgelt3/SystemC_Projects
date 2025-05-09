#ifndef ALU_H
#define ALU_H

#include <systemc>
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;


SC_MODULE(ALU) {
    sc_in<sc_int<32>> in1, in2;
    sc_in<Ops> op;
    sc_out<sc_int<32>> out;
    sc_out<bool> zero;

    SC_CTOR(ALU) {
        SC_METHOD(compute);
        sensitive << in1 << in2 << op;
    }
    

    void compute(){
        sc_int<32> a = in1.read();
        sc_int<32> b = in2.read();
        sc_int<32> result = 0;
    
        switch (op.read()) {
            case ADD: case ADDI: case LW: case SW:
                result = a + b; break;
            case SUB:
                result = a - b; break;
            case AND: case ANDI:
                result = a & b; break;
            case OR: case ORI:
                result = a | b; break;
            case XOR: case XORI:
                result = a ^ b; break;
            case SLT: case SLTI:
                result = ((sc_int<32>)a < (sc_int<32>)b); break;
            case SLL: case SLLI:
                result = a << (b & 0x1F); break;
            case SRL: case SRLI:
                result = a >> (b & 0x1F); break;
            case SRA: case SRAI:
                result = (sc_int<32>)a >> (b & 0x1F); break;
            default:
                result = 0;
        }
    
        out.write(result);
        zero.write(result == 0);
    }
    
};

#endif

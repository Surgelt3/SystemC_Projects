#ifndef ALU_H
#define ALU_H

#include <systemc>
using namespace sc_core;
using namespace sc_dt;


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

SC_MODULE(ALU) {
    sc_in<sc_uint<32>> in1, in2;
    sc_in<ALUOps> op;
    sc_out<sc_uint<32>> out;
    sc_out<bool> zero;

    SC_CTOR(ALU) {
        SC_METHOD(compute);
        sensitive << in1 << in2 << op;
    }

    void compute(){
        sc_uint<32> a = in1.read();
        sc_uint<32> b = in2.read();
        sc_uint<32> result = 0;

        switch (op.read()){
            case ALU_ADD: result = a + b; break;
            case ALU_SUB: result = a - b; break;
            case ALU_AND: result = a & b; break;
        }

        out.write(result);
        zero.write(result == 0);
        
    }

};

#endif

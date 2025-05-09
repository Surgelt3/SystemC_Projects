#ifndef IF_H
#define IF_H

#include <systemc>
#include "IMEM.h"

using namespace sc_core;
using namespace sc_dt;


SC_MODULE(IF){
   
    sc_in<bool> clk;
    sc_in<sc_uint<32>> pc_in;
    sc_out<sc_uint<32>> pc_out, instr, pc_plus4;

    
    
    IMEM* instr_mem;

    SC_CTOR(IF){
        instr_mem = new IMEM("InstructionMemory");
        instr_mem->address(pc_in);
        instr_mem->instr(instr);

        SC_METHOD(fetch);
        sensitive << pc_in; 
    }


    void fetch() {
        pc_out.write(pc_in.read());
        pc_plus4.write(inc_pc(pc_in.read()));
    }

    sc_uint<32> inc_pc(sc_uint<32> pc) {
        return pc + 4;
    }

    ~IF() {
        delete instr_mem;
    }

};

#endif
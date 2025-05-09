#ifndef DMEM_H
#define DMEM_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(DMEM){

    sc_in<bool> clk;
    sc_in<bool> WE;
    sc_in<sc_int<32>> WD, A;
    sc_out<sc_int<32>> RD;

    sc_int<32> mem[1024];

    SC_CTOR(DMEM){

        SC_METHOD(mem_read);
        SC_METHOD(mem_write);

        sensitive << clk.pos();
    }

    void mem_read(){
        if(!WE.read()){
            sc_uint<32> addr = A.read() >> 2;
            RD.write(mem[addr]);
        }
    }

    void mem_write(){
        if(WE.read()){
            sc_uint<32> addr = A.read() >> 2;
            mem[addr] = WD.read();
        }
    }

};

#endif
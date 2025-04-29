#ifndef REGFILE_H
#define REGFILE_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(REGFILE){

    sc_in<bool> clk;

    sc_in<sc_uint<32>> A1, A2, A3, WE3, WD3;
    sc_out<sc_uint<32>> RD1, RD2;

    sc_uint<32> regs[32];


    SC_CTOR(REGFILE){
        SC_METHOD(read_reg);
        SC_METHOD(write_reg);

        sensitive << clk.pos();
    }

    void read_reg(){
        if(WE3.read()==0){
            RD1.write(regs[A1.read()]);
            RD2.write(regs[A2.read()]);
        }
    }

    void write_reg(){
        if (WE3.read()){
            regs[A3.read()] = WD3.read();
        }
    }



};


#endif
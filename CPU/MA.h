#ifndef MA_H
#define MA_H

#include <systemc>
#include "DMEM.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(MA){

    sc_in<bool> clk;
    sc_in<sc_uint<1>> WE;
    sc_in<sc_uint<32>> A, WD;
    sc_out<sc_uint<32>> RD;


    *DMEM data_mem;


    SC_CTOR(MA){
        data_mem = new DMEM("DataMemory");
        data_mem->clk(clk);
        data_mem->WE(WE);
        data_mem->A(A);
        data_mem->WD(WD);
        data_mem->RD(RD);

        sensitive << clk.pos();
    }

};

#endif
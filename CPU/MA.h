#ifndef MA_H
#define MA_H

#include <systemc>
#include "DMEM.h"
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(MA){

    sc_in<bool> clk;
    sc_in<Ops> op;
    sc_in<sc_int<32>> A, WD;
    sc_out<sc_int<32>> RD;

    sc_signal<bool> WE;

    DMEM* data_mem;


    SC_CTOR(MA){

        SC_METHOD(get_WE);
        sensitive << op;

        data_mem = new DMEM("DataMemory");
        data_mem->clk(clk);
        data_mem->WE(WE);
        data_mem->A(A);
        data_mem->WD(WD);
        data_mem->RD(RD);

        sensitive << clk.pos();
    }

    void get_WE(){
        if (op.read() == SW){
            WE.write(true);
        } else{
            WE.write(false);
        }
    }



};

#endif
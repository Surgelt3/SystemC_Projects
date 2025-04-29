#ifndef ID_H
#define ID_H

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(ID){

    sc_in<bool> clk;
    sc_in<sc_uint<32>> instr;

    REGFILE* reg_file;

    SC_CTOR(ID){
        reg_file = new REGFILE("RegisterFile");
        reg_file->clk(clk);
        reg_file->A1();
        reg_file->A2();
        reg_file->A3();
        reg_file->WE3();
        reg_file->WD3();
        reg_file->RD1();
        reg_file->RD2();
    }

    void extend(){
        
    }

};


#endif
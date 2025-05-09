#ifndef WB_H
#define WB_H

#include <systemc>
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(WB) {
    sc_in<bool> PCSrc;
    sc_in<Ops> op;
    sc_in<sc_int<32>> ALU_Result, ReadData, PC_Plus4W;
    sc_in<sc_uint<32>> PC_Plus4F, PCTarget;
    sc_out<sc_int<32>> result_out;
    sc_out<sc_uint<32>> PCout;

    sc_signal<sc_uint<2>> ResultSrc;

    SC_CTOR(WB) {
        SC_METHOD(get_ResultSrc);
        sensitive << op;
    
        SC_METHOD(select_result);
        sensitive << ResultSrc << ALU_Result << ReadData << PC_Plus4W;
    
        SC_METHOD(select_pc);
        sensitive << PCSrc << PCTarget << PC_Plus4F;
    }
    
    void get_ResultSrc(){
        switch(op.read()){
            case LW: ResultSrc.write(0b01); break;
            case JAL: ResultSrc.write(0b10); break;
            case JALR: ResultSrc.write(0b10); break;
            default: ResultSrc.write(0b00);
        }
    }

    void select_result() {
        switch (ResultSrc.read()) {
            case 0b00: result_out.write(ALU_Result.read()); break;
            case 0b01: result_out.write(ReadData.read()); break;
            case 0b10: result_out.write(PC_Plus4W.read()); break;
            default: result_out.write(0); break;
        }
    }

    void select_pc() {
        if (PCSrc.read())
            PCout.write(PCTarget.read());
        else
            PCout.write(PC_Plus4F.read());
    }
};

#endif

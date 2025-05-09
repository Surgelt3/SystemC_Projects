#ifndef ID_H
#define ID_H

#include <systemc>
#include "defs.h"
#include "REGFILE.h"
#include "CNTRL_U.h"


using namespace sc_core;
using namespace sc_dt;

SC_MODULE(ID){

    sc_in<bool> clk;
    sc_in<sc_uint<32>> instr;
    sc_in<sc_int<32>> WD3;
    sc_in<Ops> opW;
    sc_in<sc_uint<5>> AW;

    sc_out<sc_int<32>> RD1, RD2, ext_out;
    sc_out<Ops> ALUop;

    sc_signal<sc_uint<5>> A1, A2;
    sc_signal<bool> WE;

    REGFILE* reg_file;
    CNTRL_U* control_unit;


    SC_CTOR(ID){

        SC_METHOD(get_WE);
        sensitive << opW;

        SC_METHOD(latch_inputs);
        sensitive << instr;

        reg_file = new REGFILE("RegisterFile");
        reg_file->clk(clk);
        reg_file->A1(A1);
        reg_file->A2(A2);
        reg_file->A3(AW);
        reg_file->WE3(WE);
        reg_file->WD3(WD3);
        reg_file->RD1(RD1);
        reg_file->RD2(RD2);
    
        control_unit = new CNTRL_U("ControlUnit");
        control_unit->instr(instr);
        control_unit->ext_out(ext_out);
        control_unit->ALUop(ALUop);

    }

    void latch_inputs() {
        A1.write(instr.read().range(19, 15));
        A2.write(instr.read().range(24, 20));
    }

    void get_WE() {
        switch(opW.read()) {
            // R-type ALU ops
            case ADD: case SUB:
            case AND: case OR:
            case XOR: case SLT:
            case SLL: case SRL: case SRA:
            // immediate ALU ops
            case ADDI: case ANDI:
            case ORI: case XORI:
            case SLTI: case SLLI:
            case SRLI: case SRAI:
            // loads and jumps write back
            case LW:
            case JAL: case JALR:
                WE.write(true);
                break;
    
            // everything else (stores, branches, NOP) do not write
            default:
                WE.write(false);
        }
    }
    
};


#endif
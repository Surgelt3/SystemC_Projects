#ifndef CPU_H
#define CPU_H

#include <systemc>
#include "defs.h"
#include "IF.h"
#include "ID.h"
#include "EX.h"
#include "MA.h"
#include "WB.h"

using namespace sc_core;
using namespace sc_dt;


SC_MODULE(CPU){

    sc_in<bool> clk, reset;

    sc_signal<sc_uint<32>> pc_in, pc_out, instr, pc_plus4;
    sc_signal<sc_uint<32>> instr_D, PCD, PCPlus4D;

    sc_signal<sc_int<32>> RD1D, RD2D, ImmExtD;
    sc_signal<Ops> opD;

    sc_signal<sc_uint<32>> instr_E, PCE, PCPlus4E, PCTargetE;
    sc_signal<sc_int<32>> RD1E, RD2E, ImmExtE, ALUResultE;
    sc_signal<Ops> opE;
    sc_signal<bool> ZeroE;

    sc_signal<bool> PCSrcE;

    sc_signal<sc_uint<32>> instr_M, PCPlus4M;
    sc_signal<sc_int<32>> ALUResultM, WriteDataM, ReadDataM;
    sc_signal<Ops> opM;

    sc_signal<sc_int<32>> ALUResultW, ReadDataW, PCPlus4W;
    sc_signal<Ops> opW;

    sc_signal<bool> RegWriteW;
    sc_signal<sc_uint<5>> RdW;
    
    sc_signal<sc_uint<32>> PCW;
    sc_signal<sc_int<32>> ResultW;

    IF* instr_f;
    ID* instr_d;
    EX* instr_ex;
    MA* instr_ma;
    WB* instr_wb;

    SC_CTOR(CPU){

        SC_METHOD(ff_if);
        sensitive << clk.pos();

        instr_f = new IF("IF_Stage");
        instr_f->clk(clk);
        instr_f->pc_in(pc_in);
        instr_f->pc_out(pc_out);
        instr_f->instr(instr);
        instr_f->pc_plus4(pc_plus4);

        SC_METHOD(ff_id);
        sensitive << clk.pos();

        instr_d = new ID("ID_Stage");
        instr_d->clk(clk);
        instr_d->instr(instr_D);
        instr_d->WD3(ResultW);
        instr_d->WE(RegWriteW);
        instr_d->AW(RdW);
        instr_d->RD1(RD1D);
        instr_d->RD2(RD2D);
        instr_d->ext_out(ImmExtD);
        instr_d->ALUop(opD);

        SC_METHOD(ff_ex);
        sensitive << clk.pos();

        instr_ex = new EX("EX_Stage");
        instr_ex->RD1(RD1E);
        instr_ex->RD2(RD2E);
        instr_ex->Imm(RD2E);
        instr_ex->PC(PCE);
        instr_ex->op(opE);
        instr_ex->Zero(ZeroE);
        instr_ex->ALUResult(ALUResultE);
        instr_ex->PCTarget(PCTargetE);

        SC_METHOD(ff_ma);
        sensitive << clk.pos();

        instr_ma = new MA("MA_Stage");
        instr_ma->clk(clk);
        instr_ma->op(opM);
        instr_ma->A(ALUResultM);
        instr_ma->WD(WriteDataM);
        instr_ma->RD(ReadDataM);
        
        SC_METHOD(ff_wb);
        sensitive << clk.pos();

        instr_wb = new WB("WB_Stage");
        instr_wb->PCSrc(PCSrcE);
        instr_wb->op(opW);
        instr_wb->ALU_Result(ALUResultW);
        instr_wb->ReadData(ReadDataW);
        instr_wb->PC_Plus4W(PCPlus4W);
        instr_wb->PC_Plus4F(pc_plus4);
        instr_wb->PCTarget(PCTargetE);
        instr_wb->result_out(ResultW);
        instr_wb->PCout(PCW);    


    }    

    void get_PCSrc(){
        Ops current_op  = opE.read();
        if ((current_op == JAL) || (current_op == JALR)){
            PCSrcE.write(true);
        } else if(ZeroE.read() && ((current_op == BEQ)||(current_op == BNE)||(current_op == BLT)||(current_op == BGE)||(current_op == BLTU)||(current_op == BGEU))){
            PCSrcE.write(true);
        } else {
            PCSrcE.write(false);
        }
    }

    void ff_if(){
        if(reset.read()){
            pc_in.write(0);
        }else{
            pc_in.write(PCW);
        }
    }

    void ff_id(){
        instr_D.write(instr.read());
        PCD.write(pc_out.read());
        PCPlus4D.write(pc_plus4.read());
    }

    void ff_ex(){
        instr_E.write(instr_D.read());
        PCE.write(PCD.read());
        PCPlus4E.write(PCPlus4D);
        RD1E.write(RD1D.read());
        RD2E.write(RD2D.read());
        ImmExtE.write(ImmExtD.read());
        opE.write(opD);
    }

    void ff_ma(){
        instr_M.write(instr_E.read());
        PCPlus4M.write(PCPlus4E.read());
        ALUResultM.write(ALUResultE.read());
        WriteDataM.write(RD2E);
        opM.write(opE.read());
    }

    void ff_wb(){
        ALUResultW.write(ALUResultM.read());
        ReadDataW.write(ReadDataM.read());
        PCPlus4W.write(static_cast<sc_int<32>>(PCPlus4M.read().to_int()));
        opW.write(opM);
        RdW.write(instr_M.read().range(11, 7));
    }

};

#endif
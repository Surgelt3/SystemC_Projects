#include <systemc>
#include "IF.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int agrc, char* argv[]){
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<sc_uint<32>> pc_in_sig, pc_out_sig, instr_sig, pc_plus4_sig;

    IF if_stage("IF");
    if_stage.clk(clk);
    if_stage.pc_in(pc_in_sig);
    if_stage.pc_out(pc_out_sig);
    if_stage.instr(instr_sig);
    if_stage.pc_plus4(pc_plus4_sig);

    if_stage.instr_mem->preload();
    pc_in_sig = 0;
    for (int i = 0; i < 5; i++){
        sc_start(10, SC_NS);
        std::cout << "Cycle " << i << ": PC input = " << pc_in_sig.read()
        << ", Instruction = 0x" << std::hex << instr_sig.read() 
        << ", PC + 4  output = " << pc_plus4_sig.read() << std::endl;
        pc_in_sig = pc_plus4_sig;
    }


    return 0;
}
#include <systemc>
#include "CNTRL_U.h"
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;



int sc_main(int agrc, char* argv[]){
    sc_signal<sc_uint<32>> instr;
    sc_signal<sc_int<32>> ext_out;
    sc_signal<Ops> ALUop;

    CNTRL_U cntrl("CNTRLU");
    cntrl.instr(instr);
    cntrl.ext_out(ext_out);
    cntrl.ALUop(ALUop);

    
        instr = 0b00000000010100011100001010010011;

        0b00000000010100011100001010010011

        sc_start(10, SC_NS);
        std::cout << "instr: " << instr.read() << ", ext_out: " << ext_out.read()
        << ", ALUop: " << ALUop.read() << std::endl;


    return 0;
}
#include <systemc>
#include "IMEM.h"

using namespace sc_core;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
    sc_signal<sc_uint<32>> pc_sig, inst_sig;
    IMEM imem("INSTRMEM");

    imem.address(pc_sig);
    imem.instr(inst_sig);

    imem.preload();

    for (int i = 0; i < 5; i++) {
        pc_sig = i*4;
        sc_start(1, SC_NS);
        std::cout << "PC=" << pc_sig.read() << ", Instruction=0x"
                  << std::hex << inst_sig.read() << std::endl;
    }
    return 0;

}
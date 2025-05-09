// MA_tb.cpp
#include <systemc>
#include "MA.h"
#include "defs.h"
#include "DMEM.h"    // for access to data_mem->mem[]

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(MA_Testbench) {
    // signals
    sc_signal<bool>        clk_sig;
    sc_signal<Ops>         op_sig;
    sc_signal<sc_int<32>>  A_sig, WD_sig;
    sc_signal<sc_int<32>>  RD_sig;

    // DUT
    MA* ma_stage;

    // clock generation: 10 ns period
    void gen_clk() {
        while (true) {
            clk_sig = false; wait(5, SC_NS);
            clk_sig = true;  wait(5, SC_NS);
        }
    }

    // test sequence
    void run() {
        // initialize
        A_sig  = 0;
        WD_sig = 0;
        op_sig = LW;
        wait(10, SC_NS);

        // 1) Store 123 at byte address 4
        A_sig  = 4;
        WD_sig = 123;
        op_sig = SW;
        wait(10, SC_NS);  // one clock

        // dump memory around index 1
        std::cout << sc_time_stamp() << " : After SW, memory contents:\n";
        for (int i = 0; i < 4; ++i) {
            std::cout << "  mem[" << i << "] = "
                      << ma_stage->data_mem->mem[i] << "\n";
        }

        // 2) Load from byte address 4
        A_sig  = 4;
        WD_sig = 0;      // ignored on read
        op_sig = LW;
        wait(10, SC_NS); // one clock

        std::cout << sc_time_stamp()
                  << " : After LW, RD = " << RD_sig.read()
                  << "\n";

        sc_stop();
    }

    SC_CTOR(MA_Testbench) {
        ma_stage = new MA("MA_Stage");
        ma_stage->clk(clk_sig);
        ma_stage->op (op_sig);
        ma_stage->A  (A_sig);
        ma_stage->WD (WD_sig);
        ma_stage->RD (RD_sig);

        SC_THREAD(gen_clk);
        SC_THREAD(run);
    }
};

int sc_main(int argc, char* argv[]) {
    MA_Testbench tb("tb");
    sc_start();
    return 0;
}

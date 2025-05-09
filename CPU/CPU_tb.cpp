#include <systemc>
#include "CPU.h"

using namespace sc_core;

SC_MODULE(TopTB) {
    sc_clock clk{"clk", 10, SC_NS};
    sc_signal<bool> reset;

    CPU* cpu;

    SC_CTOR(TopTB) {
        cpu = new CPU("cpu");
        cpu->clk(clk);
        cpu->reset(reset);

        SC_THREAD(stim);
    }

    void stim() {
        reset = true;
        wait(2, SC_NS);
        reset = false;

        // let the core run for 200 ns
        wait(200, SC_NS);
        sc_stop();
    }
};

int sc_main(int argc,char* argv[]) {
    TopTB tb("tb");
    sc_start();
    return 0;
}

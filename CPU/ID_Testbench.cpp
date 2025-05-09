//made by chat gpt

#include <systemc>
#include "ID.h"
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(ID_Testbench) {
    sc_signal<bool> clk;
    sc_signal<sc_uint<32>> instr_sig, wd3_sig;
    sc_signal<sc_uint<1>> we_sig;
    sc_signal<sc_uint<5>> aw_sig;

    sc_signal<sc_int<32>> rd1_sig, rd2_sig, ext_out_sig;
    sc_signal<Ops> aluop_sig;
    sc_signal<sc_uint<32>> instr_out_sig;

    ID* id_stage;

    SC_CTOR(ID_Testbench) {
        id_stage = new ID("ID_Stage");
        id_stage->clk(clk);
        id_stage->instr(instr_sig);
        id_stage->WD3(wd3_sig);
        id_stage->WE(we_sig);
        id_stage->AW(aw_sig);
        id_stage->RD1(rd1_sig);
        id_stage->RD2(rd2_sig);
        id_stage->ext_out(ext_out_sig);
        id_stage->ALUop(aluop_sig);
        id_stage->instr_out(instr_out_sig);

        SC_THREAD(run);
    }

    void run() {
        // Initialize signals
        clk = 0;
        instr_sig = 0;
        wd3_sig = 0;
        we_sig = 0;
        aw_sig = 0;

        wait(5, SC_NS);

        // Write 42 into register x3
        aw_sig = 3;
        wd3_sig = 42;
        we_sig = 1;

        pulse_clock(); // Perform write

        // Provide ADDI x5, x3, 5
        // imm = 5, rs1 = x3 (00011), funct3 = 000, rd = x5 (00101), opcode = 0010011
        // Binary: 00000000010100011000001010010011
        instr_sig = 0b00000000010100011000001010010011;
        we_sig = 0;

        pulse_clock(); // Latch instruction and read values

        std::cout << "\n--- ID Stage Output ---\n";
        std::cout << "Instruction: " << std::hex << instr_sig.read() << std::endl;
        std::cout << "RD1 = " << rd1_sig.read() << std::endl;
        std::cout << "RD2 = " << rd2_sig.read() << std::endl;
        std::cout << "Ext = " << ext_out_sig.read() << std::endl;
        std::cout << "ALUOp = " << aluop_sig.read() << std::endl;
        std::cout << "Instr_out = " << instr_out_sig.read() << "\n" << std::endl;

        sc_stop();
    }

    void pulse_clock() {
        clk = 1;
        wait(1, SC_NS);
        clk = 0;
        wait(1, SC_NS);
    }
};

int sc_main(int argc, char* argv[]) {
    ID_Testbench tb("tb");
    sc_start();
    return 0;
}

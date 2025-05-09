// ID_Testbench.cpp

#include <systemc>
#include "ID.h"
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;

// Helper to print the Ops enum as text:
const char* op_names[] = {
    "ADD","SUB","AND","OR","XOR","SLT","SLL","SRL","SRA","NOP",
    "ADDI","ANDI","ORI","XORI","SLTI","SLLI","SRLI","SRAI",
    "LW","SW","BEQ","BNE","BLT","BGE","BLTU","BGEU","JAL","JALR"
};

SC_MODULE(ID_Testbench) {
    // DUT I/O signals
    sc_signal<bool>         clk_sig;
    sc_signal<sc_uint<32>>  instr_sig;
    sc_signal<sc_int<32>>   wd3_sig;
    sc_signal<bool>         we_sig;
    sc_signal<sc_uint<5>>   aw_sig;
    sc_signal<sc_int<32>>   rd1_sig, rd2_sig, ext_out_sig;
    sc_signal<Ops>          aluop_sig;

    ID* id_stage;

    // 10 ns clock
    void gen_clock() {
        while (true) {
            clk_sig = false; wait(5, SC_NS);
            clk_sig = true;  wait(5, SC_NS);
        }
    }

    // drive inputs and check outputs
    void run_test() {
        // 1) initialize and write known values into register file
        instr_sig = 0; wd3_sig = 0; we_sig = false; aw_sig = 0;
        wait(10, SC_NS);

        // write x1 = 5
        aw_sig  = 1; wd3_sig = 5; we_sig = true;
        wait(10, SC_NS);
        we_sig = false;

        // write x2 = 10
        aw_sig  = 2; wd3_sig = 10; we_sig = true;
        wait(10, SC_NS);
        we_sig = false;

        // Now test every instruction in your enum:
        struct Test {
            uint32_t    word;
            const char* name;
            int         exp_rd1;
            int         exp_rd2;
            int         exp_imm;
            Ops         exp_op;
        } tests[] = {
            // R-type (rd1=5, rd2=10, imm=0)
            {0x002081B3, "ADD  x3,x1,x2", 5,10, 0, ADD},
            {0x402081b3, "SUB  x3,x1,x2", 5,10, 0, SUB},
            {0x002091b3, "SLL  x3,x1,x2", 5,10, 0, SLL},
            {0x0020A1b3, "SLT  x3,x1,x2", 5,10, 0, SLT},
            {0x0020c1b3, "XOR  x3,x1,x2", 5,10, 0, XOR},
            {0x0020f1b3, "AND  x3,x1,x2", 5,10, 0, AND},
            {0x0020e1b3, "OR   x3,x1,x2", 5,10, 0, OR },
            {0x0020d1b3, "SRL  x3,x1,x2", 5,10, 0, SRL},
            {0x4020d1b3, "SRA  x3,x1,x2", 5,10, 0, SRA},

            // I-type (rd1=5, rd2 ignored, imm=7)
            {0x00708293, "ADDI x5,x1,7",   5,10, 7, ADDI},
            {0x0070A293, "SLTI x5,x1,7",   5,10, 7, SLTI},
            {0x0070E293, "ORI  x5,x1,7",   5,10, 7, ORI },
            {0x0070F293, "ANDI x5,x1,7",   5,10, 7, ANDI},
            {0x0070C293, "XORI x5,x1,7",   5,10, 7, XORI},
            {0x00709293, "SLLI x5,x1,7",   5,10, 7, SLLI},
            {0x0070D293, "SRLI x5,x1,7",   5,10, 7, SRLI},
            {0x4070D293, "SRAI x5,x1,7",   5,10, 7, SRAI},

            // Load / Store
            {0x00112283, "LW   x5,1(x2)",   5,10, 1, LW },
            {0x00212023, "SW   x2,0(x2)",   5,10, 2, SW },

            // Branches
            {0x00410263, "BEQ  x2,x4,4",    5,10, 4, BEQ },
            {0x00411263, "BNE  x2,x4,4",    5,10, 4, BNE },
            {0x00414263, "BLT  x2,x4,4",    5,10, 4, BLT },
            {0x00415263, "BGE  x2,x4,4",    5,10, 4, BGE },
            {0x00416263, "BLTU x2,x4,4",    5,10, 4, BLTU},
            {0x00417263, "BGEU x2,x4,4",    5,10, 4, BGEU},

            // Jumps
            {0x0000006F, "JAL  x0,0",       5,10, 0, JAL  },
            {0x0040006F, "JAL  x0,4",       5,10, 4, JAL  },
            {0x00400067, "JALR x0,4(x0)",   5,10, 4, JALR }
        };

        for (auto &t : tests) {
            instr_sig = t.word;
            wait(10, SC_NS);

            bool pass = true;
            pass &= (rd1_sig.read()   == t.exp_rd1);
            pass &= (rd2_sig.read()   == t.exp_rd2);
            pass &= (ext_out_sig.read()== t.exp_imm);
            pass &= (aluop_sig.read()== t.exp_op);

            std::cout
              << t.name << " -> "
              << "RD1=" << rd1_sig.read()
              << ", RD2="  << rd2_sig.read()
              << ", IMM="  << ext_out_sig.read()
              << ", OP="   << op_names[aluop_sig.read()]
              << " ... " << (pass ? "PASS" : "FAIL")
              << std::endl;
        }

        sc_stop();
    }

    SC_CTOR(ID_Testbench) {
        // instantiate DUT
        id_stage = new ID("id_stage");
        id_stage->clk    (clk_sig);
        id_stage->instr  (instr_sig);
        id_stage->WD3    (wd3_sig);
        id_stage->opW    (aluop_sig);   // assume write-back uses prior ALUop
        id_stage->AW     (aw_sig);
        id_stage->RD1    (rd1_sig);
        id_stage->RD2    (rd2_sig);
        id_stage->ext_out(ext_out_sig);
        id_stage->ALUop  (aluop_sig);

        // processes
        SC_THREAD(gen_clock);
        SC_THREAD(run_test);
    }
};

int sc_main(int argc, char* argv[]) {
    ID_Testbench tb("tb");
    sc_start();
    return 0;
}

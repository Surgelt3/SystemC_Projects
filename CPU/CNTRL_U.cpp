#include <systemc>
#include "CNTRL_U.h"
#include "defs.h"
#include <iostream>
#include <iomanip>

using namespace sc_core;
using namespace sc_dt;

// Helpers to build 32‑bit R/I/S/B/J/JALR instructions
static uint32_t encode_r(uint8_t f7, uint8_t rs2, uint8_t rs1, uint8_t f3, uint8_t rd, uint8_t opc=0x33) {
    return (f7 << 25)
         | (rs2 << 20)
         | (rs1 << 15)
         | (f3  << 12)
         | (rd  <<  7)
         |  opc;
}
static uint32_t encode_i(int16_t imm, uint8_t rs1, uint8_t f3, uint8_t rd, uint8_t opc=0x13) {
    return ((uint32_t)(imm & 0xFFF) << 20)
         | (rs1 << 15)
         | (f3  << 12)
         | (rd  <<  7)
         |  opc;
}
static uint32_t encode_load(int16_t imm, uint8_t rs1, uint8_t f3, uint8_t rd, uint8_t opc=0x03) {
    return encode_i(imm, rs1, f3, rd, opc);
}
static uint32_t encode_s(int16_t imm, uint8_t rs2, uint8_t rs1, uint8_t f3, uint8_t opc=0x23) {
    uint32_t u = imm & 0xFFF;
    return ((u >> 5) << 25)
         | (rs2 << 20)
         | (rs1 << 15)
         | (f3  << 12)
         | ((u & 0x1F) << 7)
         |  opc;
}
static uint32_t encode_b(int16_t imm, uint8_t rs2, uint8_t rs1, uint8_t f3, uint8_t opc=0x63) {
    uint32_t u = imm & 0x1FFF;
    uint32_t b12   = (u >> 12) & 1;
    uint32_t b11   = (u >> 11) & 1;
    uint32_t b10_5 = (u >> 5 ) & 0x3F;
    uint32_t b4_1  = (u >> 1 ) & 0x0F;
    return (b12   << 31)
         | (b10_5 << 25)
         | (rs2   << 20)
         | (rs1   << 15)
         | (f3    << 12)
         | (b4_1  <<  8)
         | (b11   <<  7)
         |  opc;
}
static uint32_t encode_j(int32_t imm, uint8_t rd, uint8_t opc=0x6F) {
    uint32_t u    = imm & 0x1FFFFF;
    uint32_t b20  = (u >> 20) & 1;
    uint32_t b19_12 = (u >> 12) & 0xFF;
    uint32_t b11  = (u >> 11) & 1;
    uint32_t b10_1  = (u >> 1 ) & 0x3FF;
    return (b20    << 31)
         | (b19_12 << 12)
         | (b11    << 20)
         | (b10_1  << 21)
         | (rd     <<  7)
         |  opc;
}
static uint32_t encode_jalr(int16_t imm, uint8_t rs1, uint8_t f3, uint8_t rd, uint8_t opc=0x67) {
    return encode_i(imm, rs1, f3, rd, opc);
}

// Names for printing
const char* op_names[] = {
    "ADD","SUB","AND","OR","XOR","SLT","SLL","SRL","SRA","NOP",
    "ADDI","ANDI","ORI","XORI","SLTI","SLLI","SRLI","SRAI",
    "LW","SW","BEQ","BNE","BLT","BGE","BLTU","BGEU","JAL","JALR"
};

SC_MODULE(CNTRLU_Testbench) {
    sc_signal<sc_uint<32>> instr_sig;
    sc_signal<sc_int<32>>  ext_sig;
    sc_signal<Ops>         op_sig;

    CNTRL_U* dut;

    void run() {
        struct Test { uint32_t w; const char* name; int exp_imm; Ops exp_op; };

        Test tests[] = {
          // R-type
          { encode_r(0,2,1,0,3),        "ADD   x3,x1,x2",  0, ADD  },
          { encode_r(0x20,2,1,0,3),     "SUB   x3,x1,x2",  0, SUB  },
          { encode_r(0,2,1,1,3),        "SLL   x3,x1,x2",  0, SLL  },
          { encode_r(0,2,1,2,3),        "SLT   x3,x1,x2",  0, SLT  },
          { encode_r(0,2,1,4,3),        "XOR   x3,x1,x2",  0, XOR  },
          { encode_r(0,2,1,7,3),        "AND   x3,x1,x2",  0, AND  },
          { encode_r(0,2,1,6,3),        "OR    x3,x1,x2",  0, OR   },
          { encode_r(0,2,1,5,3),        "SRL   x3,x1,x2",  0, SRL  },
          { encode_r(0x20,2,1,5,3),     "SRA   x3,x1,x2",  0, SRA  },

          // I-type arithmetic
          { encode_i(7,1,0,5),          "ADDI  x5,x1,7",   7, ADDI },
          { encode_i(7,1,2,5),          "SLTI  x5,x1,7",   7, SLTI },
          { encode_i(7,1,6,5),          "ORI   x5,x1,7",   7, ORI  },
          { encode_i(7,1,7,5),          "ANDI  x5,x1,7",   7, ANDI },
          { encode_i(7,1,4,5),          "XORI  x5,x1,7",   7, XORI },
          { encode_i(7,1,1,5),          "SLLI  x5,x1,7",   7, SLLI },
          { encode_i((int16_t)(0x020|(1<<10)),1,5,5,0x13), "SRLI  x5,x1,7", 7, SRLI },
          { encode_i((int16_t)(0x020|(1<<10)),1,5,5,0x13 /*SRAI*/), "SRAI  x5,x1,7", 7, SRAI },

          // Loads / stores
          { encode_load(1,2,2,5),       "LW    x5,1(x2)",   1, LW   },
          { encode_s(2,2,2,2),           "SW    x2,2(x2)",   2, SW   },

          // Branches
          { encode_b(4,2,4,0,0x63),      "BEQ   x2,x4,4",    4, BEQ  },
          { encode_b(4,2,4,1,0x63),      "BNE   x2,x4,4",    4, BNE  },
          { encode_b(4,2,4,4,0x63),      "BLT   x2,x4,4",    4, BLT  },
          { encode_b(4,2,4,5,0x63),      "BGE   x2,x4,4",    4, BGE  },
          { encode_b(4,2,4,6,0x63),      "BLTU  x2,x4,4",    4, BLTU },
          { encode_b(4,2,4,7,0x63),      "BGEU  x2,x4,4",    4, BGEU },

          // Jumps
          { encode_j(0,0),               "JAL   x0,0",       0, JAL  },
          { encode_j(4,0),               "JAL   x0,4",       4, JAL  },
          { encode_jalr(4,0,0,0),        "JALR  x0,4(x0)",   4, JALR}
        };

        for (auto &t : tests) {
            instr_sig = t.w;
            wait(1, SC_NS);
            bool pass = (ext_sig.read() == t.exp_imm)
                     && (op_sig.read()  == t.exp_op);
            std::cout << std::setw(15) << t.name
                      << " (0x" << std::hex << t.w << std::dec << ")"
                      << " → IMM=" << ext_sig.read()
                      << ", OP="   << op_names[op_sig.read()]
                      << "  " << (pass ? "PASS" : "FAIL") << std::endl;
        }
        sc_stop();
    }

    SC_CTOR(CNTRLU_Testbench) {
        dut = new CNTRL_U("cntrl_u");
        dut->instr  (instr_sig);
        dut->ext_out(ext_sig);
        dut->ALUop  (op_sig);
        SC_THREAD(run);
    }
};

int sc_main(int argc, char** argv) {
    CNTRLU_Testbench tb("tb");
    sc_start();
    return 0;
}

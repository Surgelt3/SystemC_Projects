#include <systemc>
#include "ALU.h"

using namespace sc_core;
using namespace std;
using namespace sc_dt;

int sc_main(int argc, char* argv[]) {
    sc_signal<sc_uint<32>> in1_sig, in2_sig, out_sig;
    sc_signal<ALUOps> op_sig;
    sc_signal<bool> zero_sig;

    ALU alu("alu");
    alu.in1(in1_sig);
    alu.in2(in2_sig);
    alu.op(op_sig);
    alu.out(out_sig);
    alu.zero(zero_sig);

    auto test_alu = [&](uint32_t in1, uint32_t in2, ALUOps op, const string& op_name) {
        in1_sig = in1;
        in2_sig = in2;
        op_sig = op;
        sc_start(1, SC_NS);

        cout << op_name << " | "
            << "Operand1: " << in1
            << ", Operand2: " << in2
            << ", Result: " << out_sig.read()
            << ", Zero: " << zero_sig.read() << endl;
    };

    test_alu(10, 5, ALU_ADD, "ADD");

    return 0;
}


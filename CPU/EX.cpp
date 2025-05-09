#include <systemc>
#include "EX.h"
#include "defs.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(EX_Testbench) {
    sc_signal<sc_uint<32>> RD1_sig, RD2_sig, Imm_sig, PC_sig;
    sc_signal<Ops> op_sig;
    sc_signal<sc_uint<32>> ALUResult_sig, PCTarget_sig;
    sc_signal<bool> Zero_sig;

    EX* ex_stage;

    SC_CTOR(EX_Testbench) {
        ex_stage = new EX("EX_Stage");
        ex_stage->RD1(RD1_sig);
        ex_stage->RD2(RD2_sig);
        ex_stage->Imm(Imm_sig);
        ex_stage->PC(PC_sig);
        ex_stage->op(op_sig);
        ex_stage->ALUResult(ALUResult_sig);
        ex_stage->PCTarget(PCTarget_sig);
        ex_stage->Zero(Zero_sig);

        SC_THREAD(run_test);
    }

    void run_test() {
        test_case(10, 5, 100, ADD, "ADD");
        test_case(10, 5, 100, SUB, "SUB");
        test_case(0b1010, 0b1100, 100, AND, "AND");
        test_case(0b1010, 0b1100, 100, OR, "OR");
        test_case(10, 5, 100, SLT, "SLT");
        test_case(10, 1, 100, SLL, "SLL");
        test_case(10, 1, 100, SRL, "SRL");
        test_case((1 << 31), 1, 100, SRA, "SRA");

        wait(1, SC_NS);  // Ensure one last time step for final output
        sc_stop();
    }

    void test_case(uint32_t a, uint32_t b, uint32_t pc, Ops op, const std::string& name) {
        RD1_sig.write(a);
        RD2_sig.write(b);
        Imm_sig.write(b);  // use same as b for immediate
        PC_sig.write(pc);
        op_sig.write(op);

        wait(1, SC_NS);  // instead of sc_start()

        cout << name << ": RD1 = " << a
             << ", RD2 = " << b
             << ", Result = " << ALUResult_sig.read()
             << ", Zero = " << Zero_sig.read()
             << ", PC+Imm = " << PCTarget_sig.read()
             << endl;
    }
};

int sc_main(int argc, char* argv[]) {
    EX_Testbench tb("tb");
    sc_start();  // only called once here
    return 0;
}

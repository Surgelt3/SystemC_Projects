#include "systemc.h"

SC_MODULE (first_counter){
    sc_in_clk clock;
    sc_in<bool> reset;
    sc_in<bool> enable;
    sc_out<sc_uint<4>> counter_out;

    sc_uint<4> count;

    void incr_count () {
        if (reset.read()){
            count = 0;
            counter_out.write(count);
        }else if (enable.read()){
            count = count + 1;
            counter_out.write(count);
            cout << "@" << sc_time_stamp() << ":: Incremented Counter "
                << counter_out.read()<<endl;
        }
    }
    SC_CTOR(first_counter) {
        cout << "Executing new" << endl;
        SC_METHOD(incr_count);
        sensitive << reset;
        sensitive << clock.pos();
    }
};
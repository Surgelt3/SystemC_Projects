#include "systemc.h"
#include "tlm.h"
#include <tlm_utils/simple_initiator_socket.h>

SC_MODULE (AXIMaster) {
    tlm_utils::simple_initiator_socket<AXIMaster> socket;
    int id;

    SC_CTOR(AXIMaster) {
        SC_THREAD(run);
    }

    void run () {
        tlm::tlm_generic_payload trans;
        sc_time delay = sc_time(10, SC_NS);
        uint32_t data = 42;

        trans.set_address(0x1000);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
        trans.set_data_length(4);
        trans.set_write();

        std::cout << "[Master: "<< id << "] Sending Write Request to 0x1000" << std::endl;
        socket->b_transport(trans, delay);

        trans.set_address(0x4000);
        trans.set_read();
        std::cout << "[Master " << id << "] Sending Read Request to 0x4000" << std::endl;
        socket->b_transport(trans, delay);

        uint32_t read_data = *reinterpret_cast<uint32_t*>(trans.get_data_ptr());
        std::cout << "[Master " << id << "] Read Data = " << read_data << std::endl;
    }

};
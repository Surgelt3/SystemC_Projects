#include "systemc.h"
#include "tlm.h"
#include <tlm_utils/simple_target_socket.h>

SC_MODULE (AXISlave) {
    tlm_utils::simple_target_socket<AXISlave> socket;
    uint32_t memory[256];
    int id;

    SC_CTOR(AXISlave) {
        socket.register_b_transport(this, &AXISlave::b_transport);
    }

    void b_transport (tlm::tlm_generic_payload& trans, sc_time& delay) {
        uint32_t addr = trans.get_address();
        uint32_t* data_ptr = reinterpret_cast<uint32_t*>(trans.get_data_ptr());

        if (trans.is_write()){
            std::cout << "[Slave " << id << "] Writing Data " << *data_ptr << " to Address 0x" << std::hex << addr << std::endl;
            memory[addr/4] = *data_ptr;
        } else if (trans.is_read()){
            *data_ptr = memory[addr/4];
            std::cout << "[Slave " << id << "] Sending Data " << *data_ptr << " from Address 0x" << std::hex << addr << std::endl;
        }
    }
};
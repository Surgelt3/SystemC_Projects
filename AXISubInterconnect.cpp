#include "systemc.h"
#include "tlm.h"
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <map>

SC_MODULE(AXISubInterconnect){
    tlm_utils::simple_target_socket<AXISubInterconnect> master_socket;
    tlm_utils::simple_initiator_socket<AXISubInterconnect> slave_sockets[2];

    int id;

    SC_CTOR(AXISubInterconnect){
        master_socket.register_b_transport(this, &AXISubInterconnect::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay){
        uint32_t addr = trans.get_address();

        int slave_id;
        if (addr >= 0x1000 && addr < 0x2000)
            slave_id = 0;  // Slave 1
        else if (addr >= 0x2000 && addr < 0x3000)
            slave_id = 1;  // Slave 2
        else if (addr >= 0x3000 && addr < 0x4000)
            slave_id = 0;  // Slave 3
        else if (addr >= 0x4000 && addr < 0x5000)
            slave_id = 1;  // Slave 4
        else {
            std::cerr << "[Sub-Interconnect] ERROR: Invalid address 0x" << std::hex << addr << std::endl;
            return;
        }

        std::cout << "[Sub-Interconnect " << id << "] Routing address 0x" 
            << std::hex << addr << " to Slave " << slave_id << std::endl;

        slave_sockets[slave_id]->b_transport(trans, delay);
    }

};
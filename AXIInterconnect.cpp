#include "systemc.h"
#include "tlm.h"
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <map>

SC_MODULE(AXIInterconnect){

    tlm_utils::simple_target_socket<AXIInterconnect> master_sockets[2];
    tlm_utils::simple_initiator_socket<AXIInterconnect> sub_interconnect_sockets[2];

    std::map<uint32_t, int> address_map;

    SC_CTOR(AXIInterconnect) {
        master_sockets[0].register_b_transport(this, &AXIInterconnect::b_transport_0);
        master_sockets[1].register_b_transport(this, &AXIInterconnect::b_transport_1);
        
    }

    void b_transport(int master_id, tlm::tlm_generic_payload& trans, sc_time& delay) {
        uint32_t addr = trans.get_address();
        int interconnect_id;
        if (addr >= 0x1000 && addr < 0x3000)
            interconnect_id = 0; 
        else if (addr >= 0x3000 && addr < 0x5000)
            interconnect_id = 1;
        else {
            std::cerr << "[Interconnect] ERROR: Invalid address 0x" << std::hex << addr << std::endl;
            return;
        }
        sub_interconnect_sockets[interconnect_id]->b_transport(trans, delay);
    }

    void b_transport_0(tlm::tlm_generic_payload& trans, sc_time& delay) {
        b_transport(0, trans, delay);
    }

    void b_transport_1(tlm::tlm_generic_payload& trans, sc_time& delay) {
        b_transport(1, trans, delay);
    }

};
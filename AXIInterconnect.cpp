#include "systemc.h"
#include "tlm.h"
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <map>

SC_MODULE(AXIInterconnect){

    tlm_utils::simple_target_socket<AXIInterconnect> master_sockets[2];
    tlm_utils::simple_initiator_socket<AXIInterconnect> slave_sockets[2];

    std::map<uint32_t, int> address_map;

    SC_CTOR(AXIInterconnect) {
        master_sockets[0].register_b_transport(this, &AXIInterconnect::b_transport_0);
        master_sockets[1].register_b_transport(this, &AXIInterconnect::b_transport_1);

        address_map[0x1000] = 0;
        address_map[0x2000] = 1;
    }

    void b_transport(int master_id, tlm::tlm_generic_payload& trans, sc_time& delay) {
        uint32_t addr = trans.get_address();
        int slave_id = (addr < 0x2000) ? 0 : 1;
        std::cout << "[Interconnect] Master " << master_id 
                << " requested address 0x" << std::hex << addr
                << " → Forwarding to Slave " << slave_id << std::endl;

        slave_sockets[slave_id]->b_transport(trans, delay);
    }

    void b_transport_0(tlm::tlm_generic_payload& trans, sc_time& delay) {
        b_transport(0, trans, delay);
    }

    void b_transport_1(tlm::tlm_generic_payload& trans, sc_time& delay) {
        b_transport(1, trans, delay);
    }

};
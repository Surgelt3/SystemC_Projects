#include <systemc>
#include "AXIMaster.cpp"
#include "AXISlave.cpp"
#include "AXIInterconnect.cpp"

int sc_main(int argc, char* argv[]) {
    AXIMaster master1("Master 1");
    AXIMaster master2("Master 2");
    AXISlave slave1("Slave 1");
    AXISlave slave2("Slave 2");
    AXIInterconnect interconnect("Interconnect");

    master1.id = 0;
    master2.id = 1;
    slave1.id = 0;
    slave2.id = 1;

    master1.socket.bind(interconnect.master_sockets[0]);
    master2.socket.bind(interconnect.master_sockets[1]);


    interconnect.slave_sockets[0].bind(slave1.socket);
    interconnect.slave_sockets[1].bind(slave2.socket);
    
    sc_start();
    return 0;
};
#include <systemc>
#include "AXIMaster.cpp"
#include "AXISlave.cpp"
#include "AXIInterconnect.cpp"
#include "AXISubInterconnect.cpp"

int sc_main(int argc, char* argv[]) {
    AXIMaster master1("Master1");
    AXIMaster master2("Master2");

    AXIInterconnect first_level_interconnect("FirstLevelInterconnect");
    AXISubInterconnect sub_interconnect1("SubInterconnect1");
    AXISubInterconnect sub_interconnect2("SubInterconnect2");

    AXISlave slave1("Slave1");
    AXISlave slave2("Slave2");
    AXISlave slave3("Slave3");
    AXISlave slave4("Slave4");


    master1.id = 0;
    master2.id = 1;

    sub_interconnect1.id = 0;
    sub_interconnect2.id = 1;


    slave1.id = 0;
    slave2.id = 1;
    slave3.id = 0;
    slave4.id = 1;


    master1.socket.bind(first_level_interconnect.master_sockets[0]);
    master2.socket.bind(first_level_interconnect.master_sockets[1]);

    first_level_interconnect.sub_interconnect_sockets[0].bind(sub_interconnect1.master_socket);
    first_level_interconnect.sub_interconnect_sockets[1].bind(sub_interconnect2.master_socket);


    sub_interconnect1.slave_sockets[0].bind(slave1.socket);
    sub_interconnect1.slave_sockets[1].bind(slave2.socket);
    sub_interconnect2.slave_sockets[0].bind(slave3.socket);
    sub_interconnect2.slave_sockets[1].bind(slave4.socket);


    sc_start();
    return 0;
};
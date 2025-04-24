#ifndef IF_H
#define IF_H

#include <systemc>
#include <array>
#include <fstream>
#include <sstream>

using namespace sc_core;
using namespace sc_dt;


SC_MODULE(IF){
    sc_in<sc_uint<32>> address;
    sc_out<sc_uint<32>> instr;

    std::array<uint32_t, 256> mem;

    SC_CTOR(IF){
        SC_METHOD(read);
        sensitive << address;

        //preload();
    }

    void read(){
        uint32_t addr = address.read();
        instr.write(mem[addr / 4]);
    }

    void preload(){
        std::string filename = "instructions.txt";
        std::ifstream infile(filename);
        std::string line;
        uint32_t addr =0;
        
        while(std::getline(infile, line) && addr < mem.size()){
            std::stringstream ss;
            ss << std::hex << line;
            ss >> mem[addr++];
        }
        std::cout << "IMEM Loaded " << addr << " instructions from " << filename << std::endl;
    }

};

#endif

#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

#include "systemc.h"
#include "common.h"

SC_MODULE(DataMemory) {
    sc_in<address> addr_in;
    sc_in<word> data_in;
    sc_in<bool> write_enable;
    sc_out<word> data_out;

    std::vector<word> memory;

    void read_data();

    void write_data();

    SC_CTOR(DataMemory) : memory(1 << ADDR_SIZE, 0){
        SC_METHOD(read_data);
        sensitive << addr_in;
        dont_initialize();

        SC_METHOD(write_data);
        sensitive << addr_in << data_in << write_enable;
        dont_initialize();
    }
};


#endif // DATA_MEMORY_H
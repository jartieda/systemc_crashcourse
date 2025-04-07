#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include "systemc.h"
#include "common.h"
#include <string>


SC_MODULE(RegisterFile)
{
    sc_in<sc_uint<8>> read_reg1_addr, read_reg2_addr, write_reg_addr;
    sc_in<word> write_data;
    sc_in<bool> write_enable;
    sc_out<word> read_data1, read_data2;

    word registers[NUM_REGISTERS];

    void write_register();
    void read_registers();

    SC_CTOR(RegisterFile) {
        SC_METHOD(read_registers);
        sensitive << read_reg1_addr << read_reg2_addr;
        dont_initialize();

        SC_METHOD(write_register);
        sensitive << write_reg_addr << write_data << write_enable;
        dont_initialize();

        // Initialize registers to 0
        for (int i = 0; i < NUM_REGISTERS; ++i) {
            registers[i] = 0;
        }
    }


};

#endif // REGISTER_FILE_H
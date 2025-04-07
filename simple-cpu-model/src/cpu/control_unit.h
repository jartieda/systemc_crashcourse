#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include "systemc.h"
#include "common.h"

SC_MODULE(ControlUnit) {
public:
    // Input and output ports

    sc_in<word> instruction_in;
    sc_out<bool> pc_enable;
    sc_out<bool> mem_read_enable, mem_write_enable;
    sc_out<address> mem_addr;
    sc_out<word> mem_write_data;
    sc_out<sc_uint<8>> reg_read1_addr, reg_read2_addr, reg_write_addr;
    //sc_out<word> reg_write_data_from_mem; // Data from memory to register
    sc_out<bool> reg_write_enable;
    sc_out<word> alu_operand1, alu_operand2;
    sc_out<sc_uint<8>> alu_control;

    // Constructor
    SC_CTOR(ControlUnit) {
        SC_METHOD(decode);
        sensitive << instruction_in; // Sensitive to changes in instruction input
    }

    void decode() ;    
};

#endif // CONTROL_UNIT_H
#ifndef INSTRUCTION_MEMORY_H
#define INSTRUCTION_MEMORY_H

#include "systemc.h"
#include <vector>
#include "common.h"

SC_MODULE(InstructionMemory) {
    sc_in<address> addr_in;
    sc_out<word> instruction_out;

    std::vector<word> memory;

    void load_program(const std::vector<word>& program) ;
    void read_instruction();
    // Constructor
    SC_CTOR(InstructionMemory) {
        SC_METHOD(read_instruction);
        sensitive << addr_in;
        dont_initialize();
    }
};

#endif // INSTRUCTION_MEMORY_H
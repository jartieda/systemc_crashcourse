#include "instruction_memory.h"
#include <systemc.h>
#include <vector>

    void InstructionMemory::load_program(const std::vector<word>& program) {
        memory = program;
        SC_REPORT_INFO("InstructionMemory", "Program loaded.");
    }
    void InstructionMemory::read_instruction() {
        if (addr_in.read() < memory.size()) {
            instruction_out.write(memory[addr_in.read()]);
        } else {
            SC_REPORT_ERROR("InstructionMemory", "Address out of bounds!");
            instruction_out.write(0); // Or some default invalid instruction
        }
    }
    
#ifndef COMMON_H
#define COMMON_H
#include <systemc.h>
// Define word size and address size
const int WORD_SIZE = 16;
const int ADDR_SIZE = 8;
const int NUM_REGISTERS = 8;

// Define basic data types
typedef sc_uint<WORD_SIZE> word;
typedef sc_uint<ADDR_SIZE> address;

// Define some basic opcodes (for simplicity)
enum Opcode {
    LOAD = 1,
    STORE = 2,
    ADD = 3,
    SUB = 4,
    HALT = 0
};
#endif // COMMON_H
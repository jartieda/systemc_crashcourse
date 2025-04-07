#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#include "systemc.h"
#include "common.h"
SC_MODULE(ProgramCounter) {
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<bool> enable;
    sc_out<address> current_address;

    address pc;

    void increment();

    SC_CTOR(ProgramCounter) : pc(0) {
        SC_THREAD(increment);
        sensitive << clk.pos() << reset.pos();
        dont_initialize();
    }
};

#endif // PROGRAM_COUNTER_H
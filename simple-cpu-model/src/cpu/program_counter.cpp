#include "program_counter.h"


void ProgramCounter::increment() {
    while (true) {
        wait(clk.posedge_event()); // Wait for the rising edge of the clock
        if (reset.read()) {
            pc = 0;
        } else if (enable.read()) {
            pc++;
        }
        current_address.write(pc);
    }
}

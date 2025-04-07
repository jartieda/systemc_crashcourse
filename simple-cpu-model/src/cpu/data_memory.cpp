#include "data_memory.h"
#include <systemc.h>

void DataMemory::read_data() {
    data_out.write(memory[addr_in.read()]);
}
void DataMemory::write_data() {
    if (write_enable.read()) {
        memory[addr_in.read()] = data_in.read();
        SC_REPORT_INFO("DataMemory", ("Wrote " + data_in.read().to_string() + " to address " + addr_in.read().to_string()).c_str());
    }
}
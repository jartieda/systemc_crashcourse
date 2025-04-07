#include "register_file.h"

void RegisterFile::read_registers() {
    read_data1.write(registers[read_reg1_addr.read()]);
    read_data2.write(registers[read_reg2_addr.read()]);
}

void RegisterFile::write_register() {
    if (write_enable.read()) {
        registers[write_reg_addr.read()] = write_data.read();
        SC_REPORT_INFO("RegisterFile", ("Wrote " + write_data.read().to_string() + " to register " + write_reg_addr.read().to_string()).c_str());
    }

}

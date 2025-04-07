#include "control_unit.h"

void ControlUnit::decode() {
    Opcode opcode = static_cast<Opcode>((instruction_in.read().range(15, 12)).to_uint());
    sc_uint<3> rd = instruction_in.read().range(11, 9);
    sc_uint<3> rs1 = instruction_in.read().range(8, 6);
    word immediate = instruction_in.read().range(5, 0);
    sc_uint<3> rs2 = immediate.range(5, 3); // Using upper bits for second register in ALU ops

    pc_enable.write(true); // Usually increment PC

    mem_read_enable.write(false);
    mem_write_enable.write(false);

    reg_write_enable.write(false);

    alu_control.write(0); // Default to ADD

    switch (opcode) {
        case LOAD:
            mem_read_enable.write(true);
            mem_addr.write(immediate); // Address from immediate field
            reg_write_enable.write(true);
            reg_write_addr.write(rd);
            break;
        case STORE:
            mem_write_enable.write(true);
            mem_addr.write(immediate); // Address from immediate field
            mem_write_data.write(/* Need data from a register */ 0); // Will be connected later
            break;
        case ADD:
            alu_control.write(0);
            alu_operand1.write(/* Need data from rs1 */ 0); // Will be connected later
            alu_operand2.write(/* Need data from rs2 */ 0); // Will be connected later
            reg_write_enable.write(true);
            reg_write_addr.write(rd);
            break;
        case SUB:
            alu_control.write(1);
            alu_operand1.write(/* Need data from rs1 */ 0); // Will be connected later
            alu_operand2.write(/* Need data from rs2 */ 0); // Will be connected later
            reg_write_enable.write(true);
            reg_write_addr.write(rd);
            break;
        case HALT:
            SC_REPORT_INFO("ControlUnit", "HALT instruction encountered. Stopping simulation.");
            sc_stop();
            break;
        default:
            SC_REPORT_WARNING("ControlUnit", ("Unknown opcode: " + sc_uint<4>(opcode).to_string(SC_BIN)).c_str());
            break;
    }

    reg_read1_addr.write(rs1);
    reg_read2_addr.write(rs2);
    // mem_write_data will be connected to a register's output
}

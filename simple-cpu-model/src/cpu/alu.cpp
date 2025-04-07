#include "alu.h"

void ALU::perform_operation()
{
    // Perform the operation based on the control signal
    switch (alu_control.read().to_uint()) {
        case 0: // ADD
            result.write(operand1.read() + operand2.read());
            break;
        case 1: // SUB
            result.write(operand1.read() - operand2.read());
            break;
        default:
            SC_REPORT_WARNING("ALU", "Invalid ALU control signal.");
            result.write(0);
            break;
    }
}
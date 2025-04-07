#ifndef ALU_H
#define ALU_H
#include <systemc.h>
#include <string>
#include "common.h"
SC_MODULE (ALU){
public:
    sc_in<word> operand1, operand2;
    sc_in<sc_uint<8>> alu_control; // Example: 00=ADD, 01=SUB
    sc_out<word> result;
    void perform_operation(); 

    SC_CTOR(ALU) {
        SC_METHOD(perform_operation);
        sensitive << operand1 << operand2 << alu_control;
        dont_initialize();
    }
};

#endif // ALU_H
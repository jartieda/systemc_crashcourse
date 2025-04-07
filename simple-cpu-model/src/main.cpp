#include <systemc.h>
#include "cpu/instruction_memory.h"
#include "cpu/data_memory.h"
#include "cpu/program_counter.h"
#include "cpu/control_unit.h"
#include "cpu/register_file.h"
#include "cpu/alu.h"
#include "cpu/common.h"



SC_MODULE(SimpleCPU) {
    InstructionMemory imem;
    DataMemory dmem;
    ProgramCounter pc;
    ControlUnit ctrl;
    RegisterFile regfile;
    ALU alu;

    sc_clock clk;
    sc_signal<address> pc_addr;
    sc_signal<word> instruction;
    sc_signal<bool> pc_en;
    sc_signal<bool> mem_rd, mem_wr;
    sc_signal<address> mem_addr_sig;
    sc_signal<word> mem_wr_data_sig, mem_rd_data_sig;
    sc_signal<sc_uint<8>> rf_rd1_addr, rf_rd2_addr, rf_wr_addr;
    sc_signal<word> rf_rd1_data, rf_rd2_data, rf_wr_data_sig;
    sc_signal<bool> rf_wr_en;
    sc_signal<word> alu_op1, alu_op2, alu_res;
    sc_signal<sc_uint<8>> alu_ctrl_sig;
    sc_signal<bool> reset_sig; // For PC reset

    SC_CTOR(SimpleCPU) :
        imem("imem"),
        dmem("dmem"),
        pc("pc"),
        ctrl("ctrl"),
        regfile("regfile"),
        alu("alu"),
        clk("clock", 10, SC_NS),
        reset_sig("reset", true) // Initialize reset high
    {
        // Instruction Memory Connections
        imem.addr_in(pc_addr);
        imem.instruction_out(instruction);

        // Program Counter Connections
        pc.clk(clk);
        pc.reset(reset_sig);
        pc.enable(pc_en);
        pc.current_address(pc_addr);

        // Control Unit Connections
        ctrl.instruction_in(instruction);
        ctrl.pc_enable(pc_en);
        ctrl.mem_read_enable(mem_rd);
        ctrl.mem_write_enable(mem_wr);
        ctrl.mem_addr(mem_addr_sig);
        ctrl.mem_write_data(mem_wr_data_sig);
        ctrl.reg_read1_addr(rf_rd1_addr);
        ctrl.reg_read2_addr(rf_rd2_addr);
        ctrl.reg_write_addr(rf_wr_addr);
        ctrl.reg_write_enable(rf_wr_en);
        ctrl.alu_operand1(alu_op1);
        ctrl.alu_operand2(alu_op2);
        ctrl.alu_control(alu_ctrl_sig);

        // Register File Connections
        regfile.read_reg1_addr(rf_rd1_addr);
        regfile.read_reg2_addr(rf_rd2_addr);
        regfile.write_reg_addr(rf_wr_addr);
        regfile.write_data(rf_wr_data_sig);
        regfile.write_enable(rf_wr_en);
        regfile.read_data1(rf_rd1_data);
        regfile.read_data2(rf_rd2_data);

        // ALU Connections
        alu.operand1(alu_op1);
        alu.operand2(alu_op2);
        alu.alu_control(alu_ctrl_sig);
        alu.result(alu_res);

        // Data Memory Connections
        dmem.addr_in(mem_addr_sig);
        dmem.data_in(mem_wr_data_sig);
        dmem.write_enable(mem_wr);
        dmem.data_out(mem_rd_data_sig);

        // Connecting data paths based on instruction type
        SC_METHOD(connect_data_paths);
        sensitive << instruction << rf_rd1_data << rf_rd2_data << alu_res << mem_rd_data_sig;
        dont_initialize();

        // Reset the PC after a short delay
        SC_THREAD(reset_pc);
    }

    void connect_data_paths() {
        Opcode opcode = static_cast<Opcode>((instruction.read().range(15, 12)).to_uint());

        switch (opcode) {
            case LOAD:
                rf_wr_data_sig.write(mem_rd_data_sig);
                break;
            case STORE:
                mem_wr_data_sig.write(rf_rd1_data); // Assuming we store the value of rs1
                break;
            case ADD:
            case SUB:
                alu_op1.write(rf_rd1_data);
                alu_op2.write(rf_rd2_data);
                rf_wr_data_sig.write(alu_res);
                break;
            case HALT:
                break;
            default:
                break;
        }
    }

    void reset_pc() {
        wait(5, SC_NS);
        reset_sig.write(false);
    }

    void load_instruction_memory(const std::vector<word>& program) {
        imem.load_program(program);
    }

    void load_data_memory(const std::vector<word>& data) {
        for (size_t i = 0; i < data.size(); ++i) {
            dmem.memory[i] = data[i];
        }
    }
};



int sc_main(int argc, char* argv[]) {
    SimpleCPU cpu("cpu");

    // Example Program:
    // LOAD R1, 10   (Load value at address 10 into R1)
    // LOAD R2, 11   (Load value at address 11 into R2)
    // ADD R3, R1, R2 (Add R1 and R2, store result in R3)
    // STORE R

    // HALT
    std::vector<word> program = {
        0x000A0001, // LOAD R1, 10
        0x000B0002, // LOAD R2, 11
        0x000C0003, // ADD R3, R1, R2
        0x000D0004, // STORE R3, 12
        0x00000000  // HALT
    };
    std::vector<word> data = {
        0x0000000A, // Address 10
        0x0000000B, // Address 11
        0x00000000, // Address 12 (for storing result)
    };
    cpu.load_instruction_memory(program);
    cpu.load_data_memory(data);
    sc_start(100, SC_NS); // Run the simulation for 100 ns
    return 0;
}


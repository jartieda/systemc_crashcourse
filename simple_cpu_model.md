# Tutorial: Building a Simple CPU in SystemC

This tutorial will guide you through creating a rudimentary CPU model in SystemC. Our CPU will have the following basic components:

- **Instruction Memory**: Stores the program instructions.
- **Data Memory**: Stores the data the program operates on.
- **Program Counter (PC)**: Keeps track of the address of the next instruction to be executed.
- **Control Unit**: Decodes instructions and generates control signals.
- **Register File**: A small set of registers for storing temporary data.
- **Arithmetic Logic Unit (ALU)**: Performs arithmetic and logical operations.

---

## Step 1: Setting up the SystemC Environment

Make sure you have a SystemC library installed and a way to compile SystemC code (e.g., using `g++` with the OSCI SystemC library).

---

## Step 2: Defining Basic Data Types and Constants

```cpp
#include "systemc.h"
#include <vector>
#include <iostream>

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
```

---

## Step 3: Creating the Instruction Memory Module

The purpose of the instruction memory is to load and provide instructions to the CPU.



```cpp
SC_MODULE(InstructionMemory) {
    sc_in<address> addr_in;
    sc_out<word> instruction_out;

    std::vector<word> memory;

    void load_program(const std::vector<word>& program) {
        memory = program;
        SC_REPORT_INFO("InstructionMemory", "Program loaded.");
    }

    void read_instruction() {
        if (addr_in.read() < memory.size()) {
            instruction_out.write(memory[addr_in.read()]);
        } else {
            SC_REPORT_ERROR("InstructionMemory", "Address out of bounds!");
            instruction_out.write(0); // Or some default invalid instruction
        }
    }

    SC_CTOR(InstructionMemory) {
        SC_METHOD(read_instruction);
        sensitive << addr_in;
        dont_initialize();
    }
};
```

---

## Step 4: Creating the Data Memory Module

the data memory module will handle reading and writing data based on the address and control signals.

addr_in: Address input
data_in: Data input
write_enable: Control signal for writing data
data_out: Data output

```cpp
SC_MODULE(DataMemory) {
    sc_in<address> addr_in;
    sc_in<word> data_in;
    sc_in<bool> write_enable;
    sc_out<word> data_out;

    std::vector<word> memory;

    SC_CTOR(DataMemory) : memory(1 << ADDR_SIZE, 0) {} // Initialize with zeros

    void read_data() {
        data_out.write(memory[addr_in.read()]);
    }

    void write_data() {
        if (write_enable.read()) {
            memory[addr_in.read()] = data_in.read();
            SC_REPORT_INFO("DataMemory", "Wrote " + data_in.read().to_string() + " to address " + addr_in.read().to_string());
        }
    }

    SC_CTOR(DataMemory) {
        SC_METHOD(read_data);
        sensitive << addr_in;
        dont_initialize();

        SC_METHOD(write_data);
        sensitive << addr_in << data_in << write_enable;
        dont_initialize();
    }
};
```

---

## Step 5: Creating the Program Counter (PC) Module

The program counter keeps track of the current instruction address. It increments on each clock cycle unless reset.

clk: Clock input
reset: Reset signal
enable: Enable signal for incrementing the PC
current_address: Output for the current instruction address

```cpp
SC_MODULE(ProgramCounter) {
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<bool> enable;
    sc_out<address> current_address;

    address pc;

    void increment() {
        if (reset.read()) {
            pc = 0;
        } else if (enable.read()) {
            pc++;
        }
        current_address.write(pc);
    }

    SC_CTOR(ProgramCounter) : pc(0) {
        SC_THREAD(increment);
        sensitive << clk.pos() << reset.pos();
        dont_initialize();
    }
};
```

---

## Step 6: Creating the Register File Module

The register file holds a small number of registers for temporary data storage. It allows reading from and writing to registers.

read_reg1_addr: Address of the first register to read
read_reg2_addr: Address of the second register to read
write_reg_addr: Address of the register to write
write_data: Data to write to the register
write_enable: Control signal for writing data
read_data1: Output for the first register's data
read_data2: Output for the second register's data


```cpp
SC_MODULE(RegisterFile) {
    sc_in<sc_uint> read_reg1_addr, read_reg2_addr, write_reg_addr;
    sc_in<word> write_data;
    sc_in<bool> write_enable;
    sc_out<word> read_data1, read_data2;

    word registers[NUM_REGISTERS];

    void read_registers() {
        read_data1.write(registers[read_reg1_addr.read()]);
        read_data2.write(registers[read_reg2_addr.read()]);
    }

    void write_register() {
        if (write_enable.read()) {
            registers[write_reg_addr.read()] = write_data.read();
            SC_REPORT_INFO("RegisterFile", "Wrote " + write_data.read().to_string() + " to register " + write_reg_addr.read().to_string());
        }
    }

    SC_CTOR(RegisterFile) {
        SC_METHOD(read_registers);
        sensitive << read_reg1_addr << read_reg2_addr;
        dont_initialize();

        SC_METHOD(write_register);
        sensitive << write_reg_addr << write_data << write_enable;
        dont_initialize();

        // Initialize registers to 0
        for (int i = 0; i < NUM_REGISTERS; ++i) {
            registers[i] = 0;
        }
    }
};
```

---

## Step 7: Creating the ALU Module

The ALU performs arithmetic and logical operations based on control signals. For simplicity, we will implement only addition and subtraction.

operand1: First operand input
operand2: Second operand input
alu_control: Control signal for the operation (e.g., ADD, SUB)
result: Output for the result of the operation


```cpp
SC_MODULE(ALU) {
    sc_in<word> operand1, operand2;
    sc_in<sc_uint> alu_control; // Example: 00=ADD, 01=SUB
    sc_out<word> result;

    void perform_operation() {
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

    SC_CTOR(ALU) {
        SC_METHOD(perform_operation);
        sensitive << operand1 << operand2 << alu_control;
        dont_initialize();
    }
};
```

---

## Step 8: Creating the Control Unit Module

This is the most complex part, as it decodes instructions and generates control signals for other modules. For our very simple CPU, the instruction format could be:

[Opcode (4 bits)] [Register Destination (3 bits)] [Register Source 1 (3 bits)] [Register Source 2 / Immediate (6 bits)]

total: 16 bits


```cpp
SC_MODULE(ControlUnit) {
    sc_in<word> instruction_in;
    sc_out<bool> pc_enable;
    sc_out<bool> mem_read_enable, mem_write_enable;
    sc_out<address> mem_addr;
    sc_out<word> mem_write_data;
    sc_out<sc_uint> reg_read1_addr, reg_read2_addr, reg_write_addr;
    sc_out<word> reg_write_data_from_mem; // Data from memory to register
    sc_out<bool> reg_write_enable;
    sc_out<word> alu_operand1, alu_operand2;
    sc_out<sc_uint> alu_control;

    void decode() {
        Opcode opcode = static_cast<Opcode>((instruction_in.read().range(15, 12)).to_uint());
        sc_uint rd = instruction_in.read().range(11, 9);
        sc_uint rs1 = instruction_in.read().range(8, 6);
        word immediate = instruction_in.read().range(5, 0);
        sc_uint rs2 = immediate.range(5, 3); // Using upper bits for second register in ALU ops

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
                SC_REPORT_WARNING("ControlUnit", "Unknown opcode: " + sc_uint(opcode).to_string(SC_BIN));
                break;
        }

        reg_read1_addr.write(rs1);
        reg_read2_addr.write(rs2);
        // mem_write_data will be connected to a register's output
    }

    SC_CTOR(ControlUnit) {
        SC_METHOD(decode);
        sensitive << instruction_in;
        dont_initialize();
    }
};
```

---

## Step 9: Connecting the CPU Components (Top-Level Module)

this module will instantiate all the components and connect them together. It will also handle the clock and reset signals.



```cpp
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
    sc_signal<sc_uint> rf_rd1_addr, rf_rd2_addr, rf_wr_addr;
    sc_signal<word> rf_rd1_data, rf_rd2_data, rf_wr_data_sig;
    sc_signal<bool> rf_wr_en;
    sc_signal<word> alu_op1, alu_op2, alu_res;
    sc_signal<sc_uint> alu_ctrl_sig;
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
```

---

## Step 10: Creating the sc_main Function (Testbench)

This function will instantiate the CPU and load a simple program into the instruction memory. It will also run the simulation for a specified time.


``` cpp
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

```


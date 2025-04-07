# Simple CPU Model

This project implements a simple CPU model using SystemC. The CPU consists of several components that work together to execute instructions and manage data. The main components include:

- **ALU (Arithmetic Logic Unit)**: Performs arithmetic and logical operations based on control signals.
- **Control Unit**: Decodes instructions and generates control signals for other components.
- **Data Memory**: Handles reading and writing of data based on address and control signals.
- **Instruction Memory**: Loads programs and retrieves instructions based on the address input.
- **Program Counter**: Updates the current instruction address based on clock and reset signals.
- **Register File**: Manages reading and writing of registers based on control signals.

## Project Structure

The project is organized as follows:

```
simple-cpu-model
├── src
│   ├── cpu
│   │   ├── alu.cpp
│   │   ├── alu.h
│   │   ├── control_unit.cpp
│   │   ├── control_unit.h
│   │   ├── data_memory.cpp
│   │   ├── data_memory.h
│   │   ├── instruction_memory.cpp
│   │   ├── instruction_memory.h
│   │   ├── program_counter.cpp
│   │   ├── program_counter.h
│   │   ├── register_file.cpp
│   │   └── register_file.h
│   └── main.cpp
├── CMakeLists.txt
└── README.md
```

## Building the Project

To build the project, follow these steps:

1. Ensure you have CMake and a compatible C++ compiler installed.
2. Navigate to the project directory.
3. Create a build directory:
   ```
   mkdir build
   cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   make
   ```

## Running the Simulation

After building the project, you can run the simulation by executing the generated binary in the build directory. The `main.cpp` file serves as the testbench for the CPU model, instantiating the CPU components and starting the simulation.

## License

This project is open-source and available for modification and distribution under the terms of the MIT License.
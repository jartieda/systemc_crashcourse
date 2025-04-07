# Example 2: Sequential Process and Clocking 

## Code 

 
``` cpp
#include "systemc.h" 
#include <string>
//
// ASCII Block Diagram (Clocked Module):
//
//
// +-------------------+
// |                   |
// |   ClockedModule   |
// |                   |
// | clk ----+         |
// |         |         |
// | counter +------------->
// +---------|---------+
//           |         
//           +-----<---0
SC_MODULE(ClockedModule) { 
  sc_in_clk clk; 
  sc_out<int> counter; 
  int count; 
 
  // Increment the counter on the rising edge of the clock
  void increment() { 
    while (true)
    {
        if (clk.posedge()) { 
        count++; 
        counter.write(count); 
        SC_REPORT_INFO("ClockedModule", (sc_time_stamp().to_string() + ": Counter incremented to " + std::to_string(count)).c_str()); 
        } 
        wait(); // Wait for the next clock edge
    }
  } 
 
  // Constructor
  // Initialize the counter to 0 and include
  // the clock's positive edge into the sensitivity list
  SC_CTOR(ClockedModule) : count(0) { 
    SC_CTHREAD(increment, clk.pos()); 
  } 
}; 
 
// Testbench module
int sc_main(int argc, char* argv[]) {
  // Declare signals for inputs and outputs   
  ClockedModule mod("mod"); 
  sc_clock clock("my_clock", 10, SC_NS); 
  sc_signal<int> count_sig; 
 
  // Connect the clock and counter signals to the module
  mod.clk(clock); 
  mod.counter(count_sig); 
 
  // Start the clock and run the simulation
  
  sc_start(100, SC_NS); 

  return 0; 
} 
 
```



## Concept

Clock signal (sc_clock), sequential process (SC_THREAD), positive edge sensitivity (clk.pos(), sensitive << clk.pos()), time stamping (sc_time_stamp()). 

1. **Clock signal (sc_clock)**: 

The `sc_clock` is a built-in SystemC class used to generate a periodic clock signal. In the given example, `sc_clock clock("my_clock", 10, SC_NS);` creates a clock signal named "my_clock" with a period of 10 nanoseconds. 

2. **Sequential process (SC_THREAD)**: 


The `SC_CTHREAD` macro is used to define a clocked thread in a SystemC module. Threads are processes that can execute concurrently. The `SC_THREAD(increment, clk.pos());` statement starts the `increment` method as a separate thread, which will be sensitive to changes in the clock signal. 

The difference between `SC_METHOD` and `SC_THREAD` is that `SC_METHOD` don't need a while loop and is usually used for combinational logic (https://en.wikipedia.org/wiki/Combinational_logic), while `SC_CTHREAD` need a while loop and and a call to wait is required to yield the process. Clocked threads are commonly used for sequential logic (https://en.wikipedia.org/wiki/Sequential_logic). 


1. **Positive edge sensitivity (clk.pos(), sensitive << clk.pos())**: 

`clk.pos()` and `sensitive << clk.pos()` are used to make a process or thread sensitive to the positive edge (rising edge) of the clock signal. In other words, the thread or process will be triggered whenever the clock signal transitions from low to high. 

4. **Time stamping (sc_time_stamp())**: 

`sc_time_stamp()` is a function that returns the current simulation time. This is useful for logging and debugging purposes. In the example, `SC_REPORT_INFO("ClockedModule", sc_time_stamp().to_string() + ": Counter incremented to " + std::to_string(count));` generates a log message that includes the current simulation time when the counter is incremented. 


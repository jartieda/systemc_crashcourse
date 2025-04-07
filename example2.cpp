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
        SC_REPORT_INFO("ClockedModule", "Counter increment thread started");
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

#include "systemc.h" 
#include <string>
/*
ASCII Block Diagram (AND Gate Only):

+------------+
|  AND Gate  |
|            |
| a ----+    |
|       |    |
| b ----+----+-- out
+------------+
*/
SC_MODULE(DUT) { // Device Under Test (Simple AND Gate) 
  // Declare input and output ports
  sc_in<bool> a, b; 
  sc_out<bool> out; 
 
  // Process method for the AND gate
  void process() { 
    out.write(a.read() && b.read()); 
  } 
 
  // Constructor
  SC_CTOR(DUT) { 
    SC_METHOD(process); 
    sensitive << a << b; 
  } 
}; 
 
// Testbench module
SC_MODULE(Testbench) {
  // Declare signals for inputs and outputs 
  sc_signal<bool> sig_a, sig_b, sig_out; 
  DUT dut; 
 
  /*
    stimulus for the AND gate
    The testbench will apply the following test vectors:
    Time(ns) | a | b | expected out
    ----------------------
       0     | 0 | 0 |  0
       1     | 0 | 1 |  0
       2     | 1 | 0 |  0
       3     | 1 | 1 |  1
*/
  void stimulus() { 
    sig_a.write(false); sig_b.write(false); wait(1, SC_NS); 
    sig_a.write(false); sig_b.write(true);  wait(1, SC_NS); 
    sig_a.write(true);  sig_b.write(false); wait(1, SC_NS); 
    sig_a.write(true);  sig_b.write(true);  wait(1, SC_NS); 
    sc_stop(); 
  } 
 
  // Monitor the output of the DUT
  void monitor() { 
    while (true) { 
      SC_REPORT_INFO("Monitor", (sc_time_stamp().to_string() + 
                                  ": a=" + std::to_string(sig_a.read()) + 
                                  ", b=" + std::to_string(sig_b.read()) + 
                                  ", out=" + std::to_string(sig_out.read())).c_str()); 
      wait(1, SC_NS); 
    } 
  }

  // Constructor for the testbench
  SC_CTOR(Testbench) : dut("dut_instance") { 
    dut.a(sig_a); 
    dut.b(sig_b); 
    dut.out(sig_out); 
 
    SC_THREAD(stimulus); 
    SC_THREAD(monitor); 
  } 
}; 
 
// Main function
// This is the entry point of the simulation
// It creates an instance of the testbench and starts the simulation
// The testbench will generate the input signals and monitor the output
// The simulation will run until all test vectors are applied
// and the output is monitored
// The simulation will then stop
int sc_main(int argc, char* argv[]) { 
  Testbench tb("tb"); 
  sc_start(); 
  return 0; 
} 
#include "systemc.h" 
 
// This example demonstrates a simple SystemC module
SC_MODULE(SubModule) { 
  sc_in<bool> in; 
  sc_out<bool> out; 
 
  // Invert the input signal and write to the output
  void process() { 
    out.write(!in.read()); 
  } 
 

  SC_CTOR(SubModule) { 
    SC_METHOD(process); 
    sensitive << in; 
  } 
}; 
 

SC_MODULE(TopModule) { 
  sc_in<bool> top_in; 
  sc_out<bool> top_out; 
  SubModule sub; 
 
  // Constructor
  SC_CTOR(TopModule) : sub("sub_instance") { 
    sub.in(top_in); 
    sub.out(top_out); 
  } 
}; 
 

int sc_main(int argc, char* argv[]) { 
  TopModule top("top"); 
  sc_signal<bool> sig_in, sig_out; 
 
  // Connect the input and output signals to the top module
  top.top_in(sig_in); 
  top.top_out(sig_out); 
  // Initialize the input signal to true
  sig_in.write(true); 
  sc_start(1, SC_NS); 
  // Print the output signal should be false
  std::cout << "Output: " << sig_out.read() << std::endl; 
 

return 0; 
} 

#include "systemc.h" 


SC_MODULE(SimpleModule) { 
  sc_in<bool> in_port; 
  sc_out<bool> out_port; 
 

void process() { 
    out_port.write(in_port.read()); 
    SC_REPORT_INFO("SimpleModule", "Data passed through"); 
  } 
 

SC_CTOR(SimpleModule) { 
    SC_METHOD(process); 
    sensitive << in_port; 
  } 
}; 
 

int sc_main(int argc, char* argv[]) { 
  SimpleModule mod("mod"); 
  sc_signal<bool> sig; 
 
  /*
    Connecting output back to input for demonstration 

    +-------------------+
    |                   |
    |   SimpleModule    |
    |                   |
    |  in_port  out_port|
    +---|--------|------+
       |        |
       +--------+
  */


  mod.in_port(sig); 
  mod.out_port(sig); 
 
  sig.write(true); 
  sc_start(1, SC_NS); 
 
  return 0; 
} 

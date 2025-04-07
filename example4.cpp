#include "systemc.h" 
 

SC_MODULE(BufferModule) { 
  sc_in<bool> in1, in2; 
  sc_out<bool> out; 
  sc_buffer<bool> internal_buf; 
 

  void process() { 
    SC_REPORT_INFO("BufferModule", ("Processing inputs"+
                    std::to_string(in1.read()) + 
                    " and " + std::to_string(in2.read())).c_str());
    //internal_buf.write(in1.read() && in2.read()); 
    bool temp = in1.read() && in2.read();
    SC_REPORT_INFO("BufferModule", ("temp: " + 
                    std::to_string(temp)).c_str());
    internal_buf.write(temp);
    //internal_buf.write(true);
    SC_REPORT_INFO("BufferModule", ("reading buffer: " + 
                    std::to_string(internal_buf.read())).c_str());
    out.write(internal_buf.read()); 
  } 
 

  SC_CTOR(BufferModule) { 
    SC_METHOD(process); 
    sensitive << in1 << in2; 
  } 
}; 
 

int sc_main(int argc, char* argv[]) { 
  BufferModule buf("buf"); 
  sc_signal<bool> s1, s2, s_out; 
 

  buf.in1(s1); 
  buf.in2(s2); 
  buf.out(s_out); 
 

  s1.write(true); 
  s2.write(false); 
  sc_start(1, SC_NS); 
  std::cout << "Input (true, false) Output 1: " << s_out.read() << std::endl; 
 

  s1.write(true); 
  s2.write(true); 
  sc_start(1, SC_NS); 
  std::cout << "Input (true, true) Output 2: " << s_out.read() << std::endl; 
 
  return 0; 
} 

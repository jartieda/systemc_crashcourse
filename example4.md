# Example 4: Using sc_buffer 

Consider a module that processes two input signals and produces an output signal based on their logical AND operation. By utilizing sc_buffer, the internal buffer will hold and propagate the last written result, ensuring consistent and accurate signal output: 


``` cpp

#include "systemc.h" 
 

SC_MODULE(BufferModule) { 
  sc_in<bool> in1, in2; 
  sc_out<bool> out; 
  sc_buffer<bool> internal_buf; 
 

void process() { 
    internal_buf.write(in1.read() && in2.read()); 
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
  std::cout << "Output 1: " << s_out.read() << std::endl; 
 

s2.write(true); 
  sc_start(1, SC_NS); 
  std::cout << "Output 2: " << s_out.read() << std::endl; 
 

return 0; 
} 
 
```

In this example, the internal_buf is an sc_buffer that captures the logical AND of in1 and in2. This value is then written to the output signal out, demonstrating how the sc_buffer maintains the last written value and provides a reliable mechanism for signal management in your SystemC designs. 

## Concept

Using sc_buffer for signals that can have multiple drivers without explicit resolution functions (holds the last written value). 


In SystemC, the sc_buffer module is designed to handle scenarios where a signal may have multiple drivers. This module holds the value that was last written to it, making it particularly useful in systems where the signal's state needs to be preserved or updated dynamically based on the most recent input. 

### Features of sc_buffer 

Multiple Drivers: Unlike traditional signals that may not handle multiple drivers gracefully, sc_buffer can accommodate multiple sources writing to the same signal. 

Last Written Value: The value held by the sc_buffer is always the last value written to it. This behavior ensures that the most recent signal state is always available. 



# Example 3: Hierarchical Modules 

``` acii
diagram for hierarchical modules:

+-----------------+ 
| TopModule       | 
|-----------------| 
| top_in (in)   o----> sig_in (sc_signal) 
| top_out (out) o----> sig_out (sc_signal) 
|                 | 
| +-------------+ | 
| | SubModule   | | 
| |-------------| | 
| | in (in)     |<----o top_in 
| | out (out)    o----> top_out 
| +-------------+ | 
+-----------------+ 
 

```

## Code

``` cpp
 
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

```

## Concept: Creating hierarchical designs by instantiating modules within other modules. 

Concept: Creating hierarchical designs involves instantiating modules within other modules. In SystemC, this means defining a module that contains instances of other sub-modules, allowing for a structured and organized design approach. For example, you can have a `TopModule` that includes a `SubModule`, and by connecting the inputs and outputs of these modules, you can create a more complex system from simpler building blocks. 
# Example 1: Basic Module and Port Communication 

``` ascii
                +-------------------+ 
                | SimpleModule      | 
                |-------------------| 
in_port (in)  o--> sig (sc_signal) o--> out_port (out)
                +-------------------+ 
 ```
 
``` cpp
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

``` 

build with

``` bash
g++ -o example example1.cpp -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib -lsystemc -lm
```

# Concept: 

Basic module definition (SC_MODULE), input (sc_in), output (sc_out) ports, signals (sc_signal), combinational process (SC_METHOD), sensitivity list (sensitive). 

# Combinational Process 

A combinational process in digital design refers to a type of operation where the output is a direct function of the current inputs, without involving any memory elements like flip-flops or latches. This means that the output will change immediately in response to a change in any of the inputs, and there is no need for clock signals to control the operation. 

In SystemC, a combinational process is typically defined using the `SC_METHOD` macro. Here’s a brief overview: 

1. **Definition**: The process is defined within the module using the `SC_METHOD` macro. 

2. **Sensitivity List**: It is made sensitive to changes in one or more signals or ports. This is done using the `sensitive` keyword followed by the list of signals or ports to which the process should respond. This means that the process will be executed whenever there is a change in the signals listed in the sensitivity list.

3. **Behavior**: The process describes the behavior of the module. Whenever there is a change in the input signals listed in the sensitivity list, the process is executed. 

For example: 

```cpp 

SC_METHOD(combinational_process); 

sensitive << input_signal1 << input_signal2; // Sensitivity list 

``` 

In this example, `combinational_process` is the name of the method that contains the logic for the combinational process. It will be triggered whenever `input_signal1` or `input_signal2` changes. 

**Key Characteristics:** 

- No memory elements involved. 

- Output depends solely on current input values. 

- Executed whenever there is a change in the inputs it is sensitive to. 

Combinational processes are fundamental in digital design, as they are used to implement logic gates, arithmetic operations, multiplexers, demultiplexers, and various other combinational logic circuits. 
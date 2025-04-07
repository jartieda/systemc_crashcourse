# Example 5: Using sc_fifo for Communication 

``` cpp 

#include "systemc.h"
#include <string>
#include <iostream>
#include <vector>

SC_MODULE(Producer)
{
    sc_fifo_out<int> out_fifo;

    void produce()
    {
        for (int i = 0; i < 5; ++i)
        {
            wait(1, SC_NS);
            out_fifo.write(i);
            SC_REPORT_INFO("Producer", (sc_time_stamp().to_string() + ": Produced " + std::to_string(i)).c_str());
        }
    }

    SC_CTOR(Producer)
    {
        SC_THREAD(produce);
    }
};

SC_MODULE(Consumer)
{
    sc_fifo_in<int> in_fifo;

    void consume()
    {
        int data;
        while (true)
        {
            data = in_fifo.read();
            SC_REPORT_INFO("Consumer", (sc_time_stamp().to_string() + ": Consumed " + std::to_string(data)).c_str());
            wait(2, SC_NS);
        }
    }

    SC_CTOR(Consumer)
    {
        SC_THREAD(consume);
    }
};

int sc_main(int argc, char *argv[])
{
    Producer prod("prod");
    Consumer cons("cons");
    sc_fifo<int> fifo(3); // FIFO with a capacity of 3

    prod.out_fifo.bind(fifo);
    cons.in_fifo.bind(fifo);

    sc_start(20, SC_NS);

    return 0;
}
 
```

## Concept

Using sc_fifo for asynchronous communication between modules, providing buffering capabilities. 

Using the `sc_fifo` for asynchronous communication between modules provides a method to buffer data. In SystemC, an `sc_fifo` is a first-in-first-out channel that allows one or more producers to write data and one or more consumers to read data. This is particularly useful for decoupling the producer and consumer processes, allowing them to operate at different rates without blocking each other. 

When a producer writes data into the `sc_fifo`, the data is stored temporarily if the consumer is not ready to read it immediately. Conversely, when the consumer reads from the `sc_fifo`, it can continue processing even if the producer has not yet produced new data. The buffer size (capacity) of the `sc_fifo` determines the maximum number of items that can be stored at any one time. If the `sc_fifo` is full, the producer will wait until space becomes available. If it is empty, the consumer will wait until data is produced. 

This buffering mechanism simplifies the design of complex systems by managing the data flow between different modules efficiently and ensuring smooth communication without direct synchronization. 
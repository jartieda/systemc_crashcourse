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
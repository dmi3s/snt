#include "producer.hpp"

namespace sn_test {

    producer::producer(storage& stor, int hz) : gen(hz), stor(stor) {}

    void producer::exit()
    {
        quit = true;
    }

    void producer::runGenerator()
    {
        while (!quit)
        {
            stor.add(gen.newValue());
        }
        stor.flush();
    }


}
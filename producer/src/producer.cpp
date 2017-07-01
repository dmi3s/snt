#include <thread>
#include "producer.hpp"

namespace sn_test {

    producer::producer(storage& stor, int freq) : 
        stor(stor),
        period(std::chrono::milliseconds(1000) / freq),
        last_value_time(clock::duration::zero())
    {}

    int producer::getFreq() const
    {
        return static_cast<int>( std::chrono::microseconds(1000) / period );
    }

    void producer::exit()
    {
        quit = true;
    }

    void producer::run()
    {
        while (!quit)
        {
            const auto time_to_sleep = last_value_time + period - clock::now();
            if (time_to_sleep.count() > 0)
                std::this_thread::sleep_for(time_to_sleep);
            stor.add(gen.newValue());
            last_value_time = clock::now();
        }
        stor.flush();
    }


}

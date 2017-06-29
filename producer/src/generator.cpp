#include <thread>
#include "generator.hpp"

namespace sn_test {

    generator::generator(int hz) :
        gen(std::random_device()()),
        dist(std::numeric_limits<sample>::min(), std::numeric_limits<sample>::max()),
        period(std::chrono::milliseconds(1000) / hz),
        last_value_time(clock::duration::zero())
    {}

    sample generator::newValue()
    {
        const auto time_to_sleep = last_value_time + period - clock::now() ;
        if (time_to_sleep.count() > 0)
            std::this_thread::sleep_for(time_to_sleep);
        last_value_time = clock::now();
        return dist(gen);
    }


}
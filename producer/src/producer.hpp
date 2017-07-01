#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <atomic>
#include "storage.hpp"
#include "generator.hpp"

namespace sn_test {

    class producer {
    public:
        producer(storage& stor, int freq);
        void run();
        void exit();
        int getFreq() const;

        producer(const producer&) = delete;
        producer& operator=(const producer&) = delete;

    private:
        typedef std::chrono::steady_clock clock;
        const clock::duration period;
        clock::time_point last_value_time;

        generator gen;
        storage& stor;
        std::atomic<bool> quit = false;
    };

}

#endif // PRODUCER_HPP
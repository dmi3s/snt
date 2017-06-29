#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <atomic>
#include "storage.hpp"
#include "generator.hpp"

namespace sn_test {

    class producer {
    public:
        producer(storage& stor, int hz);
        void runGenerator();
        void exit();
    private:
        std::atomic<bool> quit = false;
        generator gen;
        storage& stor;
    };

}

#endif // PRODUCER_HPP
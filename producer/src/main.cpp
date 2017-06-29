#include <thread>
#include <chrono>
#include <random>
#include <limits>
#include <string>
#include <condition_variable>
#include <deque>
#include <atomic>
#include <iostream>
#include "sample.hpp"
#include "generator.hpp"
#include "producer.hpp"


namespace sn_test {

    class publisher {
    public:
        typedef std::chrono::steady_clock clock;

        publisher(storage& store, int hz);
        void getValues(clock::duration d, std::vector<sample>& dst) const;
    private:
        storage& store;
        const int hz;
    };

    publisher::publisher(storage& store, int hz) :
        store(store),
        hz(hz)
    {}

    void publisher::getValues(clock::duration /*d*/, std::vector<sample>& /*dst*/) const
    {

    }
}


using namespace std::chrono_literals;
using namespace std;
using namespace sn_test;



int main()
{
    auto const Hz = 20;

    storage store;
    producer pd(store, Hz);

    thread pd_thread(&producer::runGenerator, &pd);

    this_thread::sleep_for(4s);
    pd.exit();
    pd_thread.join();
}

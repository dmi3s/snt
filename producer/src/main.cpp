#include <thread>
#include <chrono>
#include <random>
#include <limits>
#include <string>
#include <condition_variable>
#include <deque>
#include <atomic>
#include <iostream>

#include <functional>

#include <future>

#include "sample.hpp"
#include "generator.hpp"
#include "producer.hpp"


namespace sn_test {

    class publisher {
    public:
        typedef std::chrono::steady_clock clock;

        publisher(storage& store, int hz);
        void enumValues(clock::duration d, std::function<void(sample)> fn) const;
    private:
        storage& store;
        const int hz;
    };

    publisher::publisher(storage& store, int hz) :
        store(store),
        hz(hz)
    {}

    void publisher::enumValues(clock::duration d, std::function<void (sample)> fn) const
    {
        auto const nsamples = std::chrono::duration_cast<std::chrono::milliseconds>(d).count() * hz / 1000;
        std::cout << nsamples << "\n";
        fn(10);
    }
}


using namespace std::chrono_literals;
using namespace std;
using namespace sn_test;



int main()
{
    auto const freq = 20;

    storage store;
    producer prod(store, freq);

    auto pd_ftr = async(std::launch::async, &producer::run, &prod);
    this_thread::sleep_for(4s);
    prod.exit();
    pd_ftr.wait();


    //publisher pb(store, Hz);
    //vector<sample> dst;
    //pb.enumValues(4s, [](sample s) {cout << to_string(s) << "\n"; });
}

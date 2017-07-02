#include <thread>
#include <chrono>
#include <random>
#include <limits>
#include <string>
#include <condition_variable>
#include <deque>
#include <atomic>
#include <iostream>
#include <sstream>

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


namespace {
    std::string createStoreDirName(int freq)
    {
        return "store [" + to_string(freq) + "] - " + to_string(chrono::steady_clock::now().time_since_epoch().count());
    }
}

int main()
{
    auto const freq = 50;

    storage store( createStoreDirName(freq) );
    producer signal_simulator(freq);
    signal_simulator.connect( [&](sample sm) { store.add(sm); } );

    auto store_ft = async(std::launch::async, &storage::threadFn, &store);
    auto sim_ft = async(std::launch::async, &producer::threadFn, &signal_simulator);

    this_thread::sleep_for(4s);

    signal_simulator.exit();
    store.exit();

    try {
        store_ft.get(); // wait + exception
        sim_ft.get(); 
    }
    catch (const exception& e)
    {
        cerr << "FAILURE: " << e.what() << endl;
        char ch;
        cin >> ch;
        return -1;
    }

}

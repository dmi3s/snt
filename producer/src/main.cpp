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


namespace sn_test {

    class generator {
    public:
        generator();
        sample newValue();
    private:
        std::mt19937 gen;
        std::uniform_int_distribution<sample> dist;
    };

    generator::generator() : 
        gen(std::random_device()()), 
        dist(std::numeric_limits<sample>::min(), std::numeric_limits<sample>::max())
    {}

    sample generator::newValue()
    {
        return dist(gen);
    }


    class storage {
    public:
        void add(const sample& sm);
        void flush();
    private:
        std::deque<sample> buff;
    };

    void storage::add(const sample& sm)
    {
        buff.push_front(sm);
        std::cout << sm << "\n";
    }

    void storage::flush()
    {
        std::cout << "dequeue size = " << buff.size() << "\n";
    }

    class producer {
    public:
        explicit producer(storage& stor);
        void runGenerator(int Hz);
        void exit();
    private:
        std::atomic<bool> quit = false;
        generator gen;
        storage& stor;
    };

    producer::producer(storage& stor) : stor(stor) {}

    void producer::exit()
    {
        quit = true;
    }

    void producer::runGenerator(int Hz)
    {
        using namespace std::chrono_literals;
        const auto sleep_ms = 1000ms / Hz;
        while (!quit)
        {
            std::this_thread::sleep_for(sleep_ms);
            stor.add(gen.newValue());
        }
        stor.flush();
    }

}

#include <iostream>

using namespace std::chrono_literals;
using namespace std;
using namespace sn_test;


int main()
{
    storage store;
    producer pd(store);
    thread pd_thread(&producer::runGenerator, &pd, 50);
    this_thread::sleep_for(2s);
    pd.exit();
    pd_thread.join();
}

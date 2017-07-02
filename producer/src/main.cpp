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
#include "publisher.hpp"


namespace sn_test {


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

    auto store              = make_shared<storage>( createStoreDirName(freq) );
    auto signal_simulator   = make_shared<producer>(freq);
    auto pub                = make_shared<publisher>(store, freq);

    signal_simulator->connect( [&](sample sm) { store->add(sm); } );

    pub->onExit([&]() 
    { 
        signal_simulator->exit(); 
        store->exit();
    });

    auto store_ft = async(std::launch::async, &istorage::threadFn,  store);
    auto sim_ft   = async(std::launch::async, &producer::threadFn,  signal_simulator);
    auto pub_ft   = async(std::launch::async, &publisher::threadFn, pub);

    try {
        // wait + exception
        store_ft.get(); 
        sim_ft.get(); 
        pub_ft.get();
    }
    catch (const exception& e)
    {
        cerr << "FAILURE: " << e.what() << endl;
    }
}

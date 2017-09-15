#include <iostream>
#include <future>

#include "sample.hpp"
#include "generator.hpp"
#include "producer.hpp"
#include "publisher.hpp"


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
    auto const FREQ = 50;

    auto store              = make_shared<storage>( createStoreDirName(FREQ) );
    auto signal_simulator   = make_shared<producer>(FREQ);
    auto pub                = make_shared<publisher>(store, signal_simulator->getFreq());

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
        // Yes, I know that there is no garantee about execution on different threads.
        // But in real life it is. You may say that this 'hack' or 'kostyl', but it works.
        // I'm not going to use this decisison in a real applicatitons. It looks good only for test.

        store_ft.get(); 
        sim_ft.get(); 
        pub_ft.get();
    }
    catch (const exception& e)
    {
        cerr << "FAILURE: " << e.what() << endl;
    }
}

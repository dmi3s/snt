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

    vector<thread> threads;
    threads.reserve(3);
    vector<future<void>> features;
    features.reserve(3);

    {
        auto pr = make_shared<promise<void>>();
        features.push_back(pr->get_future());
        threads.emplace_back( [pr, store]() { store->threadFn(pr); } );
    }

    {
        auto pr = make_shared<promise<void>>();
        features.push_back(pr->get_future());
        threads.emplace_back([pr, signal_simulator]() { signal_simulator->threadFn(pr); });
    }

    {
        auto pr = make_shared<promise<void>>();
        features.push_back(pr->get_future());
        threads.emplace_back([pr, pub]() { pub->threadFn(pr); });
    }

    try {
        for (auto& ft : features)
            ft.get();
    }
    catch (const exception& e)
    {
        cerr << "FAILURE: " << e.what() << endl;
    }

    for (auto& th : threads)
        th.join();
}

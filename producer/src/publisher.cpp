#include <iostream>
#include <string>
#include <vector>
#include <boost/range/adaptor/reversed.hpp>
#include "publisher.hpp"


namespace sn_test {

    using namespace std;

    publisher::publisher(std::shared_ptr<istorage> store, int freq) :
        store(store),
        freq(freq)
    {}

    void publisher::threadFn(std::shared_ptr<std::promise<void>> pr)
    {
        string s;
        while (true)
        {
            getline(cin, s);
            try {
                const size_t milliseconds = stoul(s);
                if (milliseconds == 0)
                {
                    cout << "EOF" << endl;
                    atExit();
                    pr->set_value();
                    return;
                }
                auto const nsamples = milliseconds * freq / 1000;
                const vector<sample> samples = store->getLast(nsamples);
                cout << samples.size() << "\n";
                for (auto sample : boost::adaptors::reverse(samples))
                    cout << sample << "\n";
            }
            catch (const exception& e)
            {
                pr->set_exception(make_exception_ptr(e));
                atExit();
                return;
            }
            cout << "EOS" << endl;
        }
    }

    void publisher::atExit()
    {
        on_exit();
        on_exit.disconnect_all_slots();
        store = shared_ptr<istorage>();
    }

    void publisher::onExit(std::function<void()> exit_fn)
    {
        on_exit.connect(exit_fn);
    }

}
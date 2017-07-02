#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

#pragma warning(push, 3)
#include <boost/process.hpp>
#pragma warning(pop)

#include <boost/filesystem.hpp>

#include "sample.hpp"

namespace bp = boost::process;
namespace fs = boost::filesystem;

using namespace std;
using namespace sn_test;

namespace {
    fs::path getProducer(const char* consumer)
    {
        fs::path producer(consumer);
        return producer.parent_path() / "producer.exe";
    }

    void skip2EOS(bp::ipstream& is)
    {
        string line;
        while (line != "EOS")
            getline(is, line);
    }

    vector<sample> processAnswer(const string& first_line, bp::ipstream& is)
    {
        vector<sample> samples;
        if (first_line == "EOS" ) return samples;
        try {
            size_t n_samples = stoul(first_line);
            samples.reserve(n_samples);
            string line;
            while (n_samples > 0)
            {
                getline(is, line);
                samples.push_back(stosm(line));
                --n_samples;
            }
            getline(is, line);
            if (line != "EOS")
            {
                cerr << "Error: awaiting EOS, but get " << line << "\n";
                skip2EOS(is);
            }
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what();
            skip2EOS(is);
        }
        return samples;
    }
}

int main(int /*argc*/, char* argv[]) 
{
    const auto ppath = getProducer(argv[0]);
    cout << "Starting producer \"" << ppath.string() << "\"\n";
    
    bp::opstream in;
    bp::ipstream out;

    bp::child c(ppath, bp::std_out > out, bp::std_in < in);

    if (!c.running())
    {
        cerr << "Failed." << endl;
        return 1;
    }

    cout << "Ok.\nEnter time for the last samples in milliseconds.\nEnter 0 to exit.\n";

    string str;
    while (c.running())
    {
        cout << "mS> ";
        getline(cin, str);
        in << str;
        getline(out, str);
        if (str == "EOF")
        {
            cout << "Bye-bye!\n";
            return 0;
        }
        auto samples = processAnswer(str, out);
        cout << "Number of samples: " << samples.size() << "\n";
        for (auto s : samples)
            cout << "[" << setw(3) << "]: " << setw(16) << s << "\n";
    }
}


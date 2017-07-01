#include "storage.hpp"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

namespace sn_test {

    using namespace std;
    namespace fs = boost::filesystem;

    storage::storage()
    {
    }

    void storage::exit() {
        quit = true;
    }


    void storage::runStorage()
    {

    }

    void storage::add(sample sm)
    {
        buff.push_front(sm);
        ++total_samples;

        if (buff.size() == BUF_MAX_ELEMS)
        {
            deque<sample> samples2save;
            std::swap(buff, samples2save);
            saveChunk(samples2save, total_samples);
        }
        cout << sm << "\n";
    }

    void storage::saveChunk(const std::deque<sample>& samples2save, const size_t nsamples)
    {
        const auto tmp_name = fs::unique_path();
        fstream out(tmp_name.string(), fstream::out | fstream::trunc);
        out << samples2save.size() << "\n";
        for (auto s : samples2save)
            out << s << "\n";
        out.exceptions();
        out.close();
        const auto chunk_name = to_string(nsamples) + ".chunk";
        fs::rename(tmp_name, chunk_name);
    }

    std::vector<sample> storage::getLast(size_t n_samples) const
    {
        std::vector<sample> result;
        result.reserve(n_samples);
        return result;
    }


    void storage::flush()
    {
        std::cout << "total samples = " << total_samples << "\n";
        if (!buff.empty())
        {
            deque<sample> samples2save;
            std::swap(buff, samples2save);
            saveChunk(samples2save, total_samples);
        }
    }


}
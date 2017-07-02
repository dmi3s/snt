#include <fstream>
#include "chunk_list.hpp"

namespace fs = boost::filesystem;
using namespace std;

namespace sn_test {

    chunk_list::chunk_list(const std::string& working_directory) :
        working_dir(working_directory)
    {
        if (!fs::exists(working_dir))
            fs::create_directory(working_dir);
    }

    void chunk_list::save(std::shared_ptr<std::deque<sample>> buff, size_t last_sample)
    {
        const auto tmp_name = fs::path(working_dir) / fs::unique_path();
        fstream out(tmp_name.string(), fstream::out | fstream::trunc);
        if (!out.is_open())
            throw fstream::failure("chunk_list failed to write file " + tmp_name.string());
        out << buff->size() << "\n";
        for (auto s : *buff)
            out << s << "\n";
        out.close();
        const auto chunk_name = fs::path(working_dir) / (to_wstring(last_sample) + L".chunk");
        fs::rename(tmp_name, chunk_name);

        unique_lock<mutex> lock(list_mutex);
        list.push_front(chunk{last_sample, buff->size()});
    }

    size_t chunk_list::loadChunk(std::vector<sample>& dst, size_t last_sample, size_t samples_to_load)
    {
        const auto chunk_name = fs::path(working_dir) / (to_wstring(last_sample) + L".chunk");
        fstream in(chunk_name.string(), fstream::in);
        if(!in.is_open())
            throw fstream::failure("chunk_list failed to read file " + chunk_name.string());
        string str;
        getline(in, str);
        const size_t sz = stoul(str);
        const auto nsamples = min(sz, samples_to_load);
        for (auto i = nsamples; i > 0; --i)
        {
            getline(in, str);
            dst.push_back(stosm(str));
        }
        return nsamples;
    }

    size_t chunk_list::loadLastSamples(std::vector<sample>& v, size_t n_samples)
    {
        unique_lock<mutex> lock(list_mutex);
        auto l = list;
        lock.unlock();

        size_t samples_rest = n_samples;
        for(auto i = l.begin(), e = l.end(); i!= e&& samples_rest!=0; ++i)
        {
            auto const loaded = loadChunk(v, i->last_sample, samples_rest);
            samples_rest -= loaded;
        }
        return n_samples - samples_rest;
    }

}
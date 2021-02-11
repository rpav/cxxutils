#pragma once

#include <fstream>

#include <rpav/file.hpp>
#include <rpav/log.hpp>
#include <rpav/str.hpp>

namespace rpav::file {

class simple_tfio final : public simple_tfio_base {
    path findActual(const path& p) const
    {
        auto actual = findFile(p);
        if(!actual) {
            throw open_error{str("Unable to open file ", p)};
        }
        return *actual;
    }

    size_t fileSize(const path& p) const
    {
        return fs::file_size(p);
    }

    void readChar(const path& p, char* data, size_t size) const
    {
        // note: p.string() for gcc-7 compatibility
        auto in = std::ifstream(p.string(), std::ios_base::in | std::ios_base::binary);
        if(!in) throw open_error{str("File exists, but unable to open ", p)};
        in.exceptions(std::ifstream::failbit);
        in.read((char*)data, size);
    }

public:
    void readFile(const path& p, std::vector<byte_type>& data) const override
    {
        auto actual = findActual(p);
        auto size   = fileSize(actual);
        data.resize(size);
        readChar(actual, (char*)data.data(), size);
    }

    void readFile(const path& p, std::string& data) const override
    {
        auto actual = findActual(p);
        auto size   = fileSize(actual);
        data.resize(size);
        readChar(actual, (char*)data.data(), size);
    }

    bool fileExists(const path& p) const override
    {
        return (bool)findFile(p);
    }

    std::optional<path> findFile(const path& p) const
    {
        path actual{p};
        auto it = _paths.rbegin();

        while(!fs::exists(actual)) {
            if(it == _paths.rend()) {
                return std::nullopt;
            }
            actual = *it / p;
            ++it;
        }

        return actual;
    }

    void eachFile(const path& dir, const std::function<void(const path&)>& f) const override
    {
        for(auto& p : fs::directory_iterator(dir)) {
            f(p);
        }
    }
};

} // namespace rpav::file

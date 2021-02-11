#pragma once

#include <optional>

#if defined(__cpp_lib_filesystem) || __has_include(<filesystem>)
#    include <filesystem>
namespace rpav {
namespace fs = std::filesystem;
}
#elif defined(__cpp_lib_experimental_filesystem) || __has_include(<experimental/filesystem>) \
        || __GNUC__ == 7
#    include <experimental/filesystem>
namespace rpav {
namespace fs = std::experimental::filesystem;
}
#endif

#include <stdexcept>
#include <vector>

#include <nlohmann/json.hpp>

namespace rpav::file {
using namespace rpav;

class open_error : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class tfio_error : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

/**
   Trivial file I/O.  Lets you simply read/write entire files as strings/bytes, and
   override in various ways.
*/
class tfio {
public:
    using path      = fs::path;
    using byte_type = unsigned char;
    using data      = std::vector<byte_type>;

public:
    virtual ~tfio()                                                        = default;
    virtual void readFile(const path& path, std::vector<byte_type>&) const = 0;
    virtual void readFile(const path& path, std::string&) const            = 0;
    virtual void readFile(const path& path, nlohmann::json& j) const
    {
        std::string s;
        readFile(path, s);
        j = nlohmann::json::parse(s);
    }

    virtual bool fileExists(const path& path) const = 0;

    virtual void add(const std::vector<path>& paths_) = 0;
    virtual void add(const path& p)                   = 0;

    virtual void eachFile(const path& dir, const std::function<void(const path&)>& f) const = 0;

    std::vector<byte_type> readFileBytes(const path& path) const
    {
        std::vector<byte_type> v;
        readFile(path, v);
        return v;
    }

    std::string readFileString(const path& path) const
    {
        std::string s;
        readFile(path, s);
        return s;
    }

    nlohmann::json readFileJson(const path& path) const
    {
        nlohmann::json j;
        readFile(path, j);
        return j;
    }
};

/**
   This gives you basic path handling internally.
 */
class simple_tfio_base : public tfio {

protected:
    std::vector<path> _paths;

public:
    virtual void add(const std::vector<path>& paths_)
    {
        _paths.insert(_paths.end(), paths_.begin(), paths_.end());
    }
    virtual void add(const path& p)
    {
        _paths.emplace_back(p);
    }

    virtual const std::vector<path>& paths() const
    {
        return _paths;
    }
};

} // namespace rpav::file

#pragma once

#include <vector>
#include <string_view>
#include <rpav/unicode/defs.hpp>

namespace rpav::unicode {

class ustring {
    encoding _enc{encoding::utf8};
    std::vector<unsigned char> _data;

public:
    ustring(const char* data, size_t bytes, encoding enc = encoding::utf8);
    ustring(const char* data, encoding enc = encoding::utf8);
    ustring(std::string_view sv);
};

}
#include <cstring>

#include <utf8proc.h>
#include <rpav/unicode/ustring.hpp>


rpav::unicode::ustring::ustring(const char* data, size_t bytes, rpav::unicode::encoding enc)
    : _enc{enc}
    , _data{data, data + bytes}
{

}

rpav::unicode::ustring::ustring(const char* data, rpav::unicode::encoding enc)
    : ustring(data, strlen(data), enc)
{ }

rpav::unicode::ustring::ustring(std::string_view sv)
    : ustring(sv.data(), sv.size(), encoding::utf8)
{ }

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <optional>

#include <rpav/endian.hpp>
#include <rpav/unicode/defs.hpp>

namespace rpav::unicode {
class ustringview {
    const void* _data{}; // Should be null-terminated
    size_t      _size{};

    encoding _enc{encoding::utf8}; // Bytes per codepoint, i.e. 1 = UTF-8, 2 = UTF-16, etc.
    /*endian   _end; // why was this here? */

public:
    ustringview()                       = default;
    ustringview(const ustringview&)     = default;
    ustringview(ustringview&&) noexcept = default;

    template<typename S>
    ustringview(const S* data, size_t len)
        : _data{data}, _size{len}, _enc{default_encoding<S>::getEncoding()}
    {}

    template<typename S>
    ustringview(const S* data, size_t len, encoding enc) : _data{data}, _size{len}, _enc{enc}
    {}

    template<typename S>
    ustringview(const S& s)
        : _data{s.data()}, _size{s.size()}, _enc{default_encoding<S>::getEncoding()}
    {}

    template<typename S>
    ustringview(const S& s, size_t start, size_t end)
        : _data{s.data() + start}
        , _size{end - start}
        , _enc{default_encoding<S>::getEncoding()}
    {}

    ustringview(const char* const s) : _data{s}, _size{std::strlen(s)}, _enc{encoding::utf8}
    {}

    ustringview(char* s) : ustringview((const char*)s) {}

    ustringview(std::nullptr_t) : _data{nullptr}, _size{0}, _enc{encoding::unknown} {}

    ustringview& operator=(const ustringview& usv)
    {
        _data = usv._data;
        _size = usv._size;
        _enc  = usv._enc;
        return *this;
    }

    ustringview& operator=(std::nullptr_t)
    {
        _data = nullptr;
        _size = 0;
        return *this;
    }

    ustringview& assign(const void* data, size_t len, encoding enc = encoding::utf8)
    {
        _data = data;
        _size = len;
        _enc  = enc;

        return *this;
    }

    explicit operator std::string() const
    {
        if(is_utf8()) {
            return std::string(utf8(), 0, _size);
        }

        return std::string();
    }

    explicit operator std::u16string() const
    {
        if(is_utf16()) {
            return std::u16string(utf16(), 0, _size);
        }

        return std::u16string();
    }

    explicit operator std::u32string() const
    {
        if(is_utf32()) {
            return std::u32string(utf32(), 0, _size);
        }

        return std::u32string();
    }

    // Note these are *NOT* guaranteed to be null-terminated!
    const char* utf8() const
    {
        if(_enc == encoding::utf8) {
            return (const char*)_data;
        }

        return nullptr;
    }

    const char16_t* utf16() const
    {
        if(_enc == encoding::utf16) {
            return (char16_t*)_data;
        }

        return nullptr;
    }

    const char32_t* utf32() const
    {
        if(_enc == encoding::utf32) {
            return (char32_t*)_data;
        }

        return nullptr;
    }

    const void* data() const { return _data; }
    size_t      size() const { return _size; }
    size_t      bytes() const { return _size * _enc; }

    encoding getEncoding() const { return _enc; }

    bool is_utf8() const { return _enc == encoding::utf8; }
    bool is_utf16() const { return _enc == encoding::utf16; }
    bool is_utf32() const { return _enc == encoding::utf32; }

    // FIXME: this should work on *graphemes*
    ustringview substr(size_t begin, std::optional<size_t> len_) const
    {
        size_t len{0};

        if(len_)
            len = *len_;
        else
            len = size() - begin;

        switch(getEncoding()) {
            case encoding::utf8:
                return ustringview(utf8() + begin, len);
            case encoding::utf16:
                return ustringview(utf16() + begin, len);
            case encoding::utf32:
                return ustringview(utf32() + begin, len);
            default:
                return "";
        }
    }
};

template<typename S>
inline ustringview substr(
    const S&              s,
    size_t                begin = 0,
    std::optional<size_t> len_  = std::nullopt)
{
    size_t len{0};

    if(len_) {
        len = *len_;
    } else {
        len = s.size() - begin;
    }

    return ustringview(s.data() + begin, len, default_encoding<S>::encoding());
}

template<>
inline ustringview substr<ustringview>(
    const ustringview&    s,
    size_t                begin,
    std::optional<size_t> len_)
{
    return s.substr(begin, len_);
}

} // namespace rpav::unicode

#pragma once

#include <rpav/enum.hpp>
#include <rpav/ctti.hpp>

namespace rpav::unicode {

struct encoding : public enum_class<encoding, uint8_t> {
    enum Value {
        unknown, utf8 = 1, utf16 = 2, utf32 = 4, MAX
    };

    encoding() : encoding(unknown) { }
    ENUM(encoding);

    static const auto* static_names() {
        const static rpav::enum_names<Value> names = {
            "Unknown", "UTF-8", "UTF-16", "<invalid>", "UTF-32", "MAX"
        };
        return &names;
    }
};

#define RPAV_DEFAULT_ENCODING(T, V) \
template<> \
struct default_encoding_<T> { \
    static constexpr encoding::Value value = V; \
    static encoding getEncoding() { return {value}; } \
}

template<typename T>
struct default_encoding_ {
    static constexpr encoding::Value value{encoding::unknown};
    static encoding getEncoding() { return {value}; }
};

RPAV_DEFAULT_ENCODING(std::string, encoding::utf8);
RPAV_DEFAULT_ENCODING(std::string_view, encoding::utf8);
RPAV_DEFAULT_ENCODING(std::u16string, encoding::utf16);
RPAV_DEFAULT_ENCODING(std::u16string_view, encoding::utf16);
RPAV_DEFAULT_ENCODING(std::u32string, encoding::utf32);
RPAV_DEFAULT_ENCODING(std::u32string_view, encoding::utf32);

RPAV_DEFAULT_ENCODING(signed char, encoding::utf8);
RPAV_DEFAULT_ENCODING(char, encoding::utf8);
RPAV_DEFAULT_ENCODING(unsigned char, encoding::utf8);
RPAV_DEFAULT_ENCODING(char16_t, encoding::utf16);
RPAV_DEFAULT_ENCODING(uint16_t, encoding::utf16);
RPAV_DEFAULT_ENCODING(char32_t, encoding::utf32);
RPAV_DEFAULT_ENCODING(uint32_t, encoding::utf32);

RPAV_DEFAULT_ENCODING(std::vector<uint8_t>, encoding::utf8);
RPAV_DEFAULT_ENCODING(std::vector<uint16_t>, encoding::utf16);
RPAV_DEFAULT_ENCODING(std::vector<uint32_t>, encoding::utf32);

template<typename T>
using default_encoding = default_encoding_<
    typename std::remove_cv_t<
        std::remove_reference_t<
            std::remove_pointer_t<T>
        >
    >
>;

}

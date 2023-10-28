#pragma once

#include <map>
#include <string_view>
#include <vector>
#include <cstdint>

namespace rpav {

template<typename T, T FINAL = T::MAX, size_t Length = static_cast<size_t>(FINAL) + 1>
class enum_names {
public:
    using name_vector = std::vector<std::string_view>;

private:
    name_vector _names;

public:
    template<typename... V>
    inline enum_names(V&&... rest) : _names{rest...}
    {
        static_assert(sizeof...(rest) == Length);
    }

    std::string_view operator[](const T v) const { return _names.at(static_cast<size_t>(v)); }

    const name_vector& names() const { return _names; }
};

template<typename EnumClass, typename EnumType = uint32_t>
class enum_class {
public:
    using enum_type       = EnumType;
    using enum_class_type = enum_class<EnumClass, EnumType>;

    constexpr enum_class()                    = default;
    constexpr enum_class(const enum_class& e) = default;
    constexpr enum_class(enum_type v) : _value(v) {}

protected:
    enum_type _value{};

public:
    constexpr enum_type value() const { return _value; }

    std::string_view name() const
    {
        auto* names = EnumClass::static_names();
        return (*names)[static_cast<typename EnumClass::Value>(_value)];
    }
};

} // namespace rpav

#if defined(ENUM)
#    warning ENUM is already defined as a macro before <rpav/enum.hpp>, skipping redefinition
#else
#    define ENUM(n)                                                                                                    \
        using enum_class::enum_class;                                                                                  \
        constexpr n(Value v) : enum_class(static_cast<enum_type>(v)) {}                                                \
        constexpr operator Value() const { return static_cast<Value>(_value); }

#    define ENUM_NAMES(...)                                                                                            \
        static const auto* static_names()                                                                              \
        {                                                                                                              \
            const static rpav::enum_names<Value> names = {__VA_ARGS__};                                                \
            return &names;                                                                                             \
        }

#    define ENUM_NAMES_MAX(MAX, ...)                                                                                   \
        static const auto* static_names()                                                                              \
        {                                                                                                              \
            const static rpav::enum_names<Value, MAX> names = {__VA_ARGS__};                                           \
            return &names;                                                                                             \
        }

#endif

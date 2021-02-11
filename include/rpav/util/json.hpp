#pragma once

/* These are some utilities for nlohmann/json ... make sure that's
 * in your path
 */

#include <optional>
#include <string_view>

#include <nlohmann/json.hpp>
#include <rpav/str.hpp>
#include <rpav/util/container.hpp>

namespace rpav {

template<typename T>
T j_default(const nlohmann::json& j, std::string_view attr, const T& default_ = T())
{
    auto&& it = j.find(attr);

    if(it == j.end()) return default_;

    return it->template get<T>();
}

template<typename T>
T j_require(const nlohmann::json& j, std::string_view attr, const rpav::strfun& err = rpav::strfun())
{
    auto&& it = j.find(attr);

    if(it == j.end()) {
        if(err) {
            throw std::runtime_error{rpav::str(err)};
        }
        throw std::runtime_error{rpav::str("Attribute ", attr, " required in ", j, " but not found.")};
    }

    return it->template get<T>();
}

template<typename T>
T j_require_ref(const nlohmann::json& j, std::string_view attr, const rpav::strfun& err = rpav::strfun())
{
    auto&& it = j.find(attr);

    if(it == j.end()) {
        if(err) {
            throw std::runtime_error{rpav::str(err)};
        }
        throw std::runtime_error{rpav::str("Attribute ", attr, " required in ", j, " but not found.")};
    }

    return it->template get_ref<T>();
}

template<typename T>
std::optional<T> j_maybe(const nlohmann::json& j, std::string_view attr)
{
    auto&& it = j.find(attr);

    if(it == j.end()) return std::nullopt;

    return it->template get<T>();
}

template<typename T>
T j_maybe_ptr(const nlohmann::json& j, std::string_view attr)
{
    auto&& it = j.find(attr);

    if(it == j.end()) return nullptr;

    return it->template get_ptr<T>();
}

template<typename T>
T j_maybe_ptr(nlohmann::json& j, std::string_view attr)
{
    auto&& it = j.find(attr);

    if(it == j.end()) return nullptr;

    return it->template get_ptr<T>();
}

inline nlohmann::json* findval(nlohmann::json& j, std::string_view k) {
    auto&& it = j.find(k);
    if(it != j.end()) return std::addressof(*it);
    return nullptr;
}

inline const nlohmann::json* findval(const nlohmann::json& j, std::string_view k) {
    auto&& it = j.find(k);
    if(it != j.end()) return std::addressof(*it);
    return nullptr;
}

} // namespace rpav

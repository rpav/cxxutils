#include <variant>

#include <rpav/typeutil.hpp>

namespace rpav {

template<typename T>
struct variant_index_ {};

template<typename... Ts>
struct variant_index_<std::variant<Ts...>> {
    using type = std::variant<Ts...>;

    template<typename T>
    static constexpr size_t index()
    {
        return type_index<std::variant_npos, Ts...>::template index<T>();
    }
};

template<typename T, typename... Ts>
constexpr auto variant_index(const std::variant<Ts...>&)
{
    return type_index<std::variant_npos, Ts...>::template index<T>();
}

} // namespace rpav

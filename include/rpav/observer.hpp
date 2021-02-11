#pragma once

#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include <rpav/algorithm.hpp>

namespace rpav {

template<typename...Ts>
class observable {
public:
    using id_type       = uintptr_t;
    using function_type = std::function<void(Ts...)>;
    using id_fun_pair   = std::pair<id_type, function_type>;

private:
    std::vector<id_fun_pair> _observers;

protected:
    bool _modified{true};

public:
    void modify() { _modified = true; }
    bool isModified() const { return _modified; }

    template<typename...Ss>
    void notify(Ss&&...ts) {
        if(isModified()) {
            for(auto&& p : _observers) {
                p.second(std::forward<Ss>(ts)...);
            }

            _modified = false;
        }
    }

    void add(id_type id, function_type f) { _observers.emplace_back(id, std::move(f)); }
    void add(void* ptr, function_type f) {
        add((id_type) ptr, f);
    }

    void remove(id_type id) {
        remove_if(_observers, _observers.begin(), _observers.end(),
                  [=](auto&& v) { return v.first == id; });
    }

    void remove(void *ptr) {
        remove((id_type)ptr);
    }
};

/**
 * An observed value type.
 * @tparam T
 */
template<typename T>
class observed : public observable<T&> {
    T _value;

    using parent = observable<T&>;
    using parent::notify;
    using parent::modify;

public:
    template<typename... Ts>
    explicit observed(Ts&& ... ts) : _value(std::forward<Ts>(ts)...) { }

    const T& operator*() const { return _value; }
    operator const T&() const { return _value; }

    const T* operator->() const { return &_value; }

    T& get() { this->modify(); return _value; }

    void notify() { this->notify(_value); }
};

} // namespace rpav

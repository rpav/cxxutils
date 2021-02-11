#pragma once

#include <rpav/ctti.hpp>
#include <rpav/log.hpp>

namespace rpav {

template<typename T>
class trace_lifetime {
    std::string_view _name() const {
        return ctti<T>::name();
    }

public:
    trace_lifetime() { say("trace<", _name(), ">() ", (void*)this); }
    trace_lifetime(const trace_lifetime& t)
    {
        say("trace<", _name(), ">(const trace&) ", this, "(", &t, ")");
    }
    trace_lifetime(trace_lifetime& t) { say("trace<", _name(), ">(trace&) ", this, "(", &t, ")"); }
    trace_lifetime(trace_lifetime&& t) noexcept
    {
        say("trace<", _name(), ">(trace&&) ", this, "(", &t, ")");
    }

    virtual ~trace_lifetime() noexcept { say("~trace<", _name(), ">() ", this); }

    trace_lifetime& operator=(const trace_lifetime&) = default;
    trace_lifetime& operator=(trace_lifetime&&) noexcept = default;
};

} // namespace rpav

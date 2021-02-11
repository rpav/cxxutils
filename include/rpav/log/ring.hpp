#pragma once

#include <string>
#include <rpav/log.hpp>
#include <rpav/ringbuffer.hpp>

namespace rpav {
class LogRing : public LogOutput {
    ringbuffer<std::string> _r{5};
    std::stringstream      _os;

public:
    LogRing() = default;
    LogRing(size_t n) : _r{n} { }
    LogRing(const LogRing& lr)
        : _r{lr._r}
        , _os{lr._os.str()}
    { }

    void beginEntry(LogLevel l) override {
        _os.str(std::string());
        _os << log_level_tag(l);
    }

    std::ostream& stream() override {
        return _os;
    }

    void endEntry() override {
        std::string input;

        while(std::getline(_os, input)) {
            _r.emplace_back(input);
        }

        _os.clear(std::ios_base::goodbit);
    }

    void clear() { _r.clear(); }

    const ringbuffer<std::string>& ring() const { return _r; }
};

}
#pragma once

#include <sstream>
#include <iostream>
#include <utility>

#include <rpav/str.hpp>
#include <rpav/dynamicscope.hpp>

#ifdef ANDROID
#include <android/log.h>
#endif

namespace rpav {

enum class LogLevel {
    Fatal, Error, Warn, Message, Note, Debug
};

#ifndef LOG_LEVEL
#   define LOG_LEVEL LogLevel::Debug
#endif

inline constexpr const char *log_level_tag(LogLevel L) {
    switch (L) {
    case LogLevel::Fatal:
        return "FATAL: ";
    case LogLevel::Error:
        return "Error: ";
    case LogLevel::Warn:
        return "Warning: ";
    case LogLevel::Note:
        return "Note: ";
    case LogLevel::Debug:
        return "D: ";
    default:
        return "";
    }
}


class LogOutput {
public:
    virtual ~LogOutput() = default;

    /* stream() should only be considered valid between beginEntry() and endEntry() */
    virtual void beginEntry(LogLevel) = 0;
    virtual std::ostream& stream() = 0;
    virtual void endEntry() = 0;
};

class LogOStream : public LogOutput {
    std::ostream &_os{std::cout};

public:
    LogOStream() = default;
    LogOStream(std::ostream &os) : _os(os) { }

    void beginEntry(LogLevel l) override {
        _os << log_level_tag(l);
    }
    std::ostream& stream() override { return _os; }
    void endEntry() override { _os << std::endl; }
};

inline LogOStream LOG_STDOUT{std::cout};
inline LogOStream LOG_STDERR{std::cerr};
inline DynamicScope<LogOutput*> LOG_OUTPUT{&LOG_STDOUT};


template<typename...Rest>
inline void log_print(std::ostream& os, const Rest&...args) {
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_DEBUG, ANDROID_APPNAME, "%s", stream(os, args...).c_str());
#else
    stream(os, args...);
#endif
}

template<typename...Rest>
inline void log(LogOutput& out, LogLevel L, const Rest&...args) {
    if (static_cast<int>(LOG_LEVEL) >= static_cast<int>(L)) {
        out.beginEntry(L);
        log_print(out.stream(), args...);
        out.endEntry();
    }
}

template<typename...Rest>
inline void log(LogOutput* out, LogLevel L, const Rest&...args) {
    if(!out) return;
    log(*out, L, args...);
}

inline void log_begin(LogOutput* out, LogLevel L) {
    out->beginEntry(L);
}

template<typename...Rest>
inline void log(LogOutput* out, const Rest&...args) {
    log_print(out->stream(), args...);
}

template<typename...Rest>
inline void log_end(LogOutput* out) {
    out->endEntry();
}

template<typename...Rest>
inline void flog(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Fatal, args...); }

template<typename...Rest>
inline void elog(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Error, args...); }

template<typename...Rest>
inline void wlog(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Warn, args...); }

template<typename...Rest>
inline void mlog(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Message, args...); }

template<typename...Rest>
inline void nlog(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Note, args...); }

template<typename...Rest>
inline void dlog(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Debug, args...); }

template<typename...Rest>
inline void dbg(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Debug, args...); }

template<typename...Rest>
inline void say(const Rest&...args) { log(*LOG_OUTPUT, LogLevel::Message, args...); }


template<
    typename T,
    typename...Rest,
    typename = std::enable_if_t<std::is_base_of_v<LogOutput,T>>
>
inline void flog(T& out, const Rest&...args) { log(out, LogLevel::Fatal, args...); }

template<
    typename T,
    typename...Rest,
    typename = std::enable_if_t<std::is_base_of_v<LogOutput,T>>
>
inline void elog(T& out, const Rest&...args) { log(out, LogLevel::Error, args...); }

template<
    typename T,
    typename...Rest,
    typename = std::enable_if_t<std::is_base_of_v<LogOutput,T>>
>
inline void wlog(T& out, const Rest&...args) { log(out, LogLevel::Warn, args...); }

template<
    typename T,
    typename...Rest,
    typename = std::enable_if_t<std::is_base_of_v<LogOutput,T>>
>
inline void mlog(T& out, const Rest&...args) { log(out, LogLevel::Message, args...); }

template<
    typename T,
    typename...Rest,
    typename = std::enable_if_t<std::is_base_of_v<LogOutput,T>>
>
inline void nlog(T& out, const Rest&...args) { log(out, LogLevel::Note, args...); }

template<
    typename T,
    typename...Rest,
    typename = std::enable_if_t<std::is_base_of_v<LogOutput,T>>
>
inline void dlog(T& out, const Rest&...args) { log(out, LogLevel::Debug, args...); }

template<
    typename T,
    typename...Rest,
    typename = std::enable_if_t<std::is_base_of_v<LogOutput,T>>
>
inline void say(T& out, const Rest&...args) { log(out, LogLevel::Message, args...); }


} /* namespace rpav */

#pragma once

#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

#include <rpav/ctti.hpp>

namespace rpav {

template<typename T>
struct str_adl {
    static void toStream(std::ostream& s, const T& v)
    {
        maybeOStream(s, v, '\0');
    }

private:
    /* Hack adapted from:
     * https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence#257382
     */
    template<typename S = T>
    static auto maybeOStream(std::ostream& s, const S& v, char) -> decltype(s << v, void())
    {
        s << v;
    }

    template<typename S>
    static auto maybeOStream(std::ostream& s, const S& v, int)
    {
        s << "#<" << ctti<T>::name() << ":" << (void*)&v << ">";
    }
};

template<>
struct str_adl<std::string> {
    static inline void toStream(std::ostream& ss, const std::string& s)
    {
        ss << s;
    }
};

template<>
struct str_adl<std::string_view> {
    static inline void toStream(std::ostream& ss, const std::string_view& sv)
    {
        ss << sv;
    }
};

template<typename T>
inline void toStream(std::ostream& os, const T& arg)
{
    str_adl<T>::toStream(os, arg);
}

inline void stream(std::ostream&) {}

template<typename First, typename... Rest>
inline void stream(std::ostream& os, const First& arg, const Rest&... args)
{
    toStream(os, arg);
    stream(os, args...);
}

template<typename... Rest>
inline std::string str(const Rest&... args)
{
    std::stringstream ss;
    stream(ss, args...);
    return ss.str();
}

/*
 This is a hack to allow semi-lazy concatenation/evaluation.
 You can do this a couple different ways:

 void foo(bool b, strfun s) {
   if(b) say(s);
 }

 void bar(bool b) {
   int i = 0;

   foo(b, strcall("foo", "bar"));      // only concatenates if b
   foo(b, strcall("foo", intval()));   // always calls intval() !

   // only call intval() if b
   foo(b, [&] { return str("foo", intval()); });

   // beware side effects:
   foo(b, strcall("foo", i++));             // always i++
   foo(b, [&] { return str("foo", i++); }); // sometimes i++
 }

*/
using strfun = std::function<std::string(void)>;

template<>
struct str_adl<const strfun&> {
    static void toStream(std::ostream& s, const strfun& f)
    {
        s << f();
    }
};

template<typename... Rest>
inline strfun strcall(Rest&&... args)
{
    return [&] { return str(std::forward<Rest>(args)...); };
}

} // namespace rpav

#define MAKE_TOSTREAM(T, OBNAME) inline void toStream(std::ostream& s, const T& OBNAME)

#define MAKE_DEFAULT_TOSTREAM(T)                                                                        \
    MAKE_TOSTREAM(T, o)                                                                                 \
    {                                                                                                   \
        rpav::stream(s, "<" #T " ", (void*)&o, ">");                                                    \
    }

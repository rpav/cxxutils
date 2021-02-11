#pragma once

#include <rpav/str.hpp>
#include <gk/core.hpp>

namespace gk {
// todo: generalize
inline void toStream(std::ostream &ss, const vec2 &v) {
    rpav::stream(ss, "(", v.x, ",", v.y, ")");
}

inline void toStream(std::ostream &ss, const ivec2 &v) {
    rpav::stream(ss, "(", v.x, ",", v.y, ")");
}

inline void toStream(std::ostream& ss, const vec3& v)
{
    rpav::stream(ss, "(", v.x, ",", v.y, ",", v.z, ")");
}

inline void toStream(std::ostream& ss, const vec4& v)
{
    rpav::stream(ss, "(", v.x, ",", v.y, ",", v.z, ",", v.w, ")");
}

inline void toStream(std::ostream& ss, const rect& v)
{
    rpav::stream(ss, "[", v.pos.x, ",", v.pos.y, " ", v.size.x, "x", v.size.y, "]");
}

inline void toStream(std::ostream& s, const mat4& m)
{
    rpav::stream(s, m.row0(), "\n", m.row1(), "\n", m.row2(), "\n", m.row3(), "\n");
}

} // namespace gk

#include <glm/mat4x4.hpp>
#include <rpav/str.hpp>

namespace rpav {

template<>
struct str_adl<const glm::mat4&> {
    static void toStream(std::ostream& s, const glm::mat4& m) {
        stream(s, "(", m[0][0], ", ", m[1][0], ", ", m[2][0], ", ", m[3][0], ")\n");
        stream(s, "(", m[0][1], ", ", m[1][1], ", ", m[2][1], ", ", m[3][1], ")\n");
        stream(s, "(", m[0][2], ", ", m[1][2], ", ", m[2][2], ", ", m[3][2], ")\n");
        stream(s, "(", m[0][3], ", ", m[1][3], ", ", m[2][3], ", ", m[3][3], ")\n");
    }
};

template<>
struct str_adl<const glm::vec4&> {
    static void toStream(std::ostream& s, const glm::vec4& v) {
        stream(s, "[", v.x, ", ", v.y, ", ", v.z, ", ", v.w, "]");
    }
};

}
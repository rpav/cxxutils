#pragma once

#include <rpav/unicode/ustringview.hpp>
#include <rpav/str.hpp>

namespace rpav::unicode {

inline void toStream(std::ostream &os, const ustringview &us) {
    if(us.is_utf8()) {
        os.write(us.utf8(), us.size());
    } else {
        stream(os, "<", us.getEncoding().name(), " string>");
    }
}

}

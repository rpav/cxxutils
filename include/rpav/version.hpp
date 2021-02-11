#pragma once

namespace rpav {

struct Version {
    int Major, Minor, Sub;

    Version(int _major, int _minor, int _sub)
        : Major(_major), Minor(_minor), Sub(_sub) { }

    inline bool operator<(const Version &v) const {
        return (Major < v.Major) || (Major == v.Major && (Minor < v.Minor)) || (Major == v.Major && Minor == v.Minor && Sub < v.Sub);
    }

    inline bool operator==(const Version &v) const {
        return Major == v.Major && Minor == v.Minor && Sub == v.Sub;
    }
};

}
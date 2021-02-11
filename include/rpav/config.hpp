#pragma once

namespace rpav {

namespace config {
#ifndef NDEBUG
constexpr bool isDebug = true;
#else
constexpr bool isDebug = false;
#endif

#ifdef RPAV_PTR_ALWAYS_CHECK
namespace defined { constexpr bool ptrAlwaysCheck = true; }
#else
namespace defined { constexpr bool ptrAlwaysCheck = false; }
#endif

constexpr bool ptrCheck = (isDebug || defined::ptrAlwaysCheck);

}

}
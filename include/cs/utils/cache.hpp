#ifndef CS_CACHE_HPP
#define CS_CACHE_HPP

#include <new>

namespace cs::details {
#ifdef __cpp_lib_hardware_interference_size
constexpr unsigned cacheLineSize = std::hardware_constructive_interference_size;
#else
constexpr unsigned cacheLineSize = 64;
#endif
}

#define cacheline_aligned alignas(cs::details::cacheLineSize)

#endif // CS_CACHE_HPP

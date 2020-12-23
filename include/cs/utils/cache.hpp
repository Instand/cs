#ifndef CS_CACHE_HPP
#define CS_CACHE_HPP

#include <new>

#ifdef __cpp_lib_hardware_interference_size
constexpr unsigned kCacheLineSize = std::hardware_constructive_interference_size;
#else
constexpr unsigned kCacheLineSize = 64;
#endif

#define cacheline_aligned alignas(kCacheLineSize)

#endif // CS_CACHE_HPP

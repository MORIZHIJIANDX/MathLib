#pragma once

#include <cstddef>
#include <assert.h>
#include <cinttypes>
#include <cstring>


#if defined(__SSE__) || (_M_IX86_FP >= 1) || defined(_M_X64)
#   define USE_SSE 1
#endif

#define USE_IEEE_754 1
#define USE_ROUNDING_CONTROL 0

#ifndef ASSERT
#   if defined(NDEBUG) && (_MSC_VER >= 1200) 
#      define ASSERT(expr) __assume(expr) 
#   else
#      define ASSERT(expr) assert(expr)
#   endif
#endif

#define STATIC_ASSERT(expr) static_assert(expr, "Compile-time assertion failed: " #expr)

#ifndef FORCEINLINE
#   if (_MSC_VER >= 1200)
#      define FORCEINLINE __forceinline
#   elif defined(__GNUC__) 
#      define FORCEINLINE __attribute__((__always_inline__)) inline
#   else
#      define FORCEINLINE inline
#   endif
#endif

#ifndef UNUSED
#   define UNUSED(arg) 
#endif

template<typename T>
void WriteData(const T& src, void* dest, std::size_t offset = 0)
{
	std::memcpy(static_cast<uint8_t*>(dest) + offset, &src, sizeof(T));
}

template<typename T>
void GetData(T& dest, void* src, std::size_t offset = 0)
{
	std::memcpy(&dest, static_cast<uint8_t*>(src) + offset, sizeof(T));
}
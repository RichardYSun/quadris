#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <type_traits>

/*
 * Contains some general utilities that other modules may share.
 */

// Convert enum to integer
// From: https://stackoverflow.com/a/45632890
template<typename T> constexpr auto enum_to_int(T value) {
  return static_cast<std::underlying_type_t<T>>(value);
}
template<typename T> constexpr auto enum_to_size_t(T value) {
  return static_cast<size_t>(value);
}

#endif

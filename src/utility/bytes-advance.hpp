#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>

namespace vdot::util {

namespace traits {

// clang-format off
template<std::size_t LEN>
struct pick_uint {
  using type = void;
};

template<> struct pick_uint<1> { using type = uint8_t;  };
template<> struct pick_uint<2> { using type = uint16_t; };
template<> struct pick_uint<4> { using type = uint32_t; };
template<> struct pick_uint<8> { using type = uint64_t; };

// clang-format on

} // namespace traits

template<class T, std::endian Endian = std::endian::big>
inline auto advance(uint8_t const *&ptr) noexcept -> T {
  if constexpr (sizeof(T) > 1) {
    T value;
    if constexpr (std::endian::native == Endian) {
      value = *reinterpret_cast<T const *>(ptr);
    } else {
      value = std::byteswap<T>(*reinterpret_cast<T const *>(ptr));
    }
    ptr += sizeof(T);
    return value;
  } else {
    return static_cast<T>(*ptr++);
  }
}

template<class T, std::size_t N, std::endian Endian = std::endian::big>
inline void advance_to(
  std::array<T, N> &buf,
  uint8_t const   *&ptr,
  std::size_t const len = N
) noexcept {
  if constexpr (auto const length{ std::min(len, N) }; sizeof(T) > 1) {
    using uint_type = typename traits::pick_uint<sizeof(T)>::type;

    union marshel {
      uint_type ui{};
      T         target;

      inline static auto cast(uint_type const value) noexcept {
        return marshel{ .ui = value }.target;
      }
    };

    for (std::size_t i{}; i < length; ++i) {
      buf[i] = marshel::cast(advance<uint_type, Endian>(ptr));
    }
  } else {
    std::copy_n(reinterpret_cast<T const *>(ptr), length, std::begin(buf));
    ptr += len;
  }
}

} // namespace vdot::util

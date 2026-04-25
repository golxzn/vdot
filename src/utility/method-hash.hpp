#pragma once

#include <cstdint>
#include <typeinfo>

namespace vdot {

    template <class T, class Ret, class... Args>
    inline static auto make_hash( Ret ( T::*method )( Args... ) ) -> uint32_t {
        return typeid( T ).hash_code() ^ typeid( decltype( method ) ).hash_code();
    }

    template <class T, class Ret, class... Args>
    inline static auto make_hash( Ret ( T::*method )( Args... ) const ) -> uint32_t {
        return typeid( T ).hash_code() ^ typeid( decltype( method ) ).hash_code();
    }

} // namespace vdot

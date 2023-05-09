#pragma once

#include <type_traits>
#include <memory>
#include <string_view>
#include "pod_vector.h"

// Tests whether compiler has c++23 support
#if (defined(__cplusplus) && __cplusplus > 202002L) || \
    (defined(_MSC_VER) && _MSC_VER > 1934 &&           \
     ((defined(_HAS_CXX23) && _HAS_CXX23 == 1) || (defined(_MSVC_LANG) && (_MSVC_LANG > 202002L))))
#    ifndef _AX_HAS_CXX23
#        define _AX_HAS_CXX23 1
#    endif  // C++23 features macro
#endif      // C++23 features check
#if !defined(_AX_HAS_CXX23)
#    define _AX_HAS_CXX23 0
#endif

namespace axstd
{
/* make_unique_for_overwrite since c++20, but not all platformm support */
template <class _Ty, std::enable_if_t<!std::is_array_v<_Ty>, int> = 0>
inline std::unique_ptr<_Ty> make_unique_for_overwrite()
{
    // make a unique_ptr with default initialization
    return std::unique_ptr<_Ty>(new _Ty);
}

template <class _Ty, std::enable_if_t<std::is_array_v<_Ty> && std::is_trivially_copyable_v<_Ty>, int> = 0>
inline std::unique_ptr<_Ty> make_unique_for_overwrite(const size_t _Size)
{
    // make a unique_ptr with default initialization
    using _Elem = std::remove_extent_t<_Ty>;
    return std::unique_ptr<_Ty>(new _Elem[_Size]);
}

// If the _OutCont is `std::vector`, the resize still with zero filled which is not needs at here
// some follow concepts needs standardized in the future, may be c++23
//  - resize_and_overwrite:
//    - https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2021/p1072r10.html
//  - boost vector resize with init behavior parameter
//    - https://github.com/boostorg/container/blob/develop/include/boost/container/vector.hpp
//  - pod_vector
//    -
//    https://stackoverflow.com/questions/15219984/using-vectorchar-as-a-buffer-without-initializing-it-on-resize/15220853#15220853
//    - https://github.com/yasio/yasio/blob/perftest/tests/perf/pod_vector.h
template <class _InIt, class _OutCont, class _Fn>
inline auto resize_and_transform(const _InIt _First, const _InIt _Last, _OutCont& _Dest, _Fn _Func)
{
    auto count = std::distance(_First, _Last);
    _Dest.resize(static_cast<size_t>(count));
    return std::transform(_First, _Last, _Dest.begin(), _Func);
}
template <class _Ty, class _InIt, class _Fn>
inline pod_vector<_Ty> pod_vector_from(const _InIt _First, const _InIt _Last, _Fn _Func)
{
    auto count = std::distance(_First, _Last);
    pod_vector<_Ty> dest(static_cast<size_t>(count));
    std::transform(_First, _Last, dest.begin(), _Func);
    return dest;
}

template <typename _CStr, typename _Fn>
inline void split_cb(_CStr s, size_t slen, typename std::remove_pointer<_CStr>::type delim, _Fn&& func)
{
    auto _Start = s;  // the start of every string
    auto _Ptr   = s;  // source string iterator
    auto _End   = s + slen;
    while ((_Ptr = strchr(_Ptr, delim)))
    {
        if (_Ptr >= _End)
            break;

        if (_Start <= _Ptr)
            func(_Start, _Ptr);
        _Start = _Ptr + 1;
        ++_Ptr;
    }
    if (_Start <= _End)
        func(_Start, _End);
}

template <typename _Fn>
inline void split_cb(std::string_view s, char delim, _Fn&& func)
{
    split_cb(s.data(), s.length(), delim, std::move(func));
}

}  // namespace axstd

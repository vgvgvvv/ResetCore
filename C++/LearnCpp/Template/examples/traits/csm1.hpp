/* The following code example is taken from the book
 * "C++ Templates - The Complete Guide"
 * by David Vandevoorde and Nicolai M. Josuttis, Addison-Wesley, 2002
 *
 * (C) Copyright David Vandevoorde and Nicolai M. Josuttis 2002.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <new>
#include <cassert>
#include <stddef.h>
#include "rparam.hpp"

// primary template
template<typename T, bool Bitwise>
class BitOrClassCSM;

// partial specialization for safe copying of objects
template<typename T>
class BitOrClassCSM<T, false> {
  public:
    static void copy (typename RParam<T>::Type src, T* dst) {
        // copy one item onto another one
        *dst = src;
    }

    static void copy_n (T const* src, T* dst, size_t n) {
        // copy n items onto n other ones
        for (size_t k = 0; k<n; ++k) {
            dst[k] = src[k];
        }
    }

    static void copy_init (typename RParam<T>::Type src,
                           void* dst) {
        // copy an item onto uninitialized storage
        ::new(dst) T(src);
    }

    static void copy_init_n (T const* src, void* dst, size_t n) {
        // copy n items onto uninitialized storage
        for (size_t k = 0; k<n; ++k) {
            ::new((void*)((T*)dst+k)) T(src[k]);
        }
    }

    static void swap (T* a, T* b) {
        // swap two items
        T tmp(*a);
        *a = *b;
        *b = tmp;
    }

    static void swap_n (T* a, T* b, size_t n) {
        // swap n items
        for (size_t k = 0; k<n; ++k) {
            T tmp(a[k]);
            a[k] = b[k];
            b[k] = tmp;
        }
    }

    static void move (T* src, T* dst) {
        // move one item onto another
        assert(src != dst);
        *dst = *src;
        src->~T();
    }

    static void move_n (T* src, T* dst, size_t n) {
        // move n items onto n other ones
        assert(src != dst);
        for (size_t k = 0; k<n; ++k) {
            dst[k] = src[k];
            src[k].~T();
        }
    }

    static void move_init (T* src, void* dst) {
        // move an item onto uninitialized storage
        assert(src != dst);
        ::new(dst) T(*src);
        src->~T();
    }

    static void move_init_n (T const* src, void* dst, size_t n) {
        // move n items onto uninitialized storage
        assert(src != dst);
        for (size_t k = 0; k<n; ++k) {
            ::new((void*)((T*)dst+k)) T(src[k]);
            src[k].~T();
        }
    }
};

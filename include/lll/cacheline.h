#ifndef __LLL_CACHELINE_H__
#define __LLL_CACHELINE_H__
/***********************************************************************
 * Low Latency Library: Cacheline aware datastructures
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include <array>
#include <cstdint>

#include "lll/constants.h"

namespace lll {

template <typename T>
class alignas(CACHELINE_SIZE) cacheline {
    union {
        T _val;
        char _[CACHELINE_SIZE];
    };

public:
    
    cacheline() : _val{0} {}

    operator T() const
    {
        return _val;
    }

    cacheline<T>&
    operator=(cacheline<T>& other)
    {
        _val = other._val;
        return *this;
    }
    
    cacheline<T>&
    operator=(const cacheline<T>& other)
    {
        _val = other._val;
        return *this;
    }
    
    cacheline<T>&
    operator=(T val)
    {
        _val = val;
        return *this;
    }

    cacheline<T>&
    operator++()
    {
        printf("in operator++ val is %lu\n", _val);
        _val++;
        return *this;
    }
    
    cacheline<T>&
    operator++(int z)
    {
        printf("in operator++ int z is %d, val is %llu\n", z, _val);
        _val++;
        return *this;
    }
    
};

} // namespace lll

#endif


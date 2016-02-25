#ifndef __LLL_OBJECT_POOL_H__
#define __LLL_OBJECT_POOL_H__
/***********************************************************************
 * Low Latency Library: Object Pool
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include <memory>

#include "astack.h"

namespace lll {

template <typename T>
class object_pool {
    astack<T> pool_;
    size_t increment_;
public:
    object_pool(size_t initial, size_t increment) : increment_(increment){
        for (size_t i=0; i<initial; i++) {
            pool_.push(new T());
        }
    };

    ~object_pool() {
        while (T* obj = pool_.pop())
            delete obj;
    }

    void operator()(T *obj) {
        pool_.push(obj);
    }

    std::unique_ptr<T> alloc() {
        return std::unique_ptr<T>(pool_.pop());
    }
};

}

#endif /* __LLL_OBJECT_POOL_H__ */

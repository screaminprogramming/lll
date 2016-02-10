#ifndef __LLL_ASTACK_H__
#define __LLL_ASTACK_H__
/***********************************************************************
 * Low Latency Library: Atomic Stack
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/

namespace lll {
template <typename T>
class astack {
    std::atomic<T *> head_;

public:
    astack() : head_() {}

    void push(T *entry) {
	T *oldhead = nullptr;
	do {
	    oldhead = head_.load();
	    entry->link = oldhead;
	} while (!head_.compare_exchange_weak(oldhead, entry));
    }

    T* pop() {
	T *ret = nullptr;
	do {
	    ret = head_.load();
	    if (ret == nullptr)
		return ret;
	} while (!head_.compare_exchange_weak(ret, ret->link));

	return ret;
    }
};

} // Namespace lll

#endif /* __LLL_ASTACK_H__ */

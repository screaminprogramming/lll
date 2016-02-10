/***********************************************************************
 * Low Latency Library: Test Program
 *
 * This is used just to test basic compilation and function of the LLL
 * library.  This should move to unit tests at some point
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include <cassert>
#include <atomic>
#include <type_traits>

#include "lll/logger.h"
#include "lll/cacheline.h"
#include "lll/astack.h"
#include "lll/object_pool.h"

int main(int, char **) {

    ////////////////////////////////////////////////////
    char buffer[58]; 
    char output[256];
    
    memset(buffer, 0x00, sizeof(buffer));
    
    size_t logsize = lll_log(buffer, sizeof(buffer), "got [%s] [%u] [%s]", "XXXX", 2, "YYYY");
    
    if (!logsize)
        printf("Buffer too small\n");
    else
	lll::logging::logformat(buffer, output, sizeof(output));
    
    printf("[%lu] %s\n", logsize, output);
    
    lll::cacheline<uint64_t> val;

    assert(val == 0);

    val = 99;

    uint64_t x = val;

    assert(x == 99);

    val++;

    assert(val == 100);

    lll::cacheline<std::atomic<uint64_t>> val2;

    class Foo {
	int val_;
    public:
	Foo() : val_(1) {}
	Foo *link;
	int get() { return val_; }
    };
    
    lll::astack<Foo> stack;
    stack.push(new Foo());
    stack.pop();

    class Blah {
    public:
	Blah() {};
    };

    lll::object_pool<Foo>  pool(10,10);

    auto pobj = pool.alloc();

    printf("Size of unique_ptr<Foo> is %lu\n", sizeof(pobj));
    
}



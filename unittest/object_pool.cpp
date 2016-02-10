#include "lll/object_pool.h"
#include "lll/lll_unittest.h"

namespace lll {
namespace unittest {

static UnitTest t(__FILE__,[](){
    class Foo {
	int val_;
    public:
	Foo() : val_(1) {}
	Foo *link;
	int get() { return val_; }
    };

    lll::object_pool<Foo>  pool(10,10);

    auto pobj = pool.alloc();

    assert(sizeof(pobj) == sizeof(void *));

    });
}
}

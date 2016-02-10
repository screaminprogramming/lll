#include "lll/astack.h"
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

    lll::astack<Foo> stack;
    stack.push(new Foo());
    stack.pop();
    });

}
}

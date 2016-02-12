#ifndef __LLL_UNITTEST_H__
#define __LLL_UNITTEST_H__
/***********************************************************************
 * Low Latency Library: Test framwork
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include <cstdio>
#include <cassert>
namespace lll {
namespace unittest {

class UnitTest;

extern UnitTest *lll_unittest_list;

class UnitTest {
    const char *name_;
    void (*fcn_)(void);
    UnitTest *next_;
public:
    UnitTest(const char *name, void (*fcn)(void)) :
	name_(name),
	fcn_(fcn)
    {
	next_ = lll_unittest_list;
	lll_unittest_list = this;
    }

    void run(void) const {
	std::printf("Starting  testcase %s\n", name_);
	fcn_();
	std::printf("Completed testcase %s\n", name_);
    }

    UnitTest *next() { return next_; }
};

} // test
} // lll

#endif 

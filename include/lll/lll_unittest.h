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
#include <exception>
#include <sstream>

namespace lll {
namespace unittest {

class UnitTest;

extern UnitTest *lll_unittest_list;

class UnitTest {
    const char *name_;
    void (*fcn_)(void);
    UnitTest *next_;
public:
    class expect_error : public std::runtime_error {
    public:
        expect_error(const std::string s) : std::runtime_error(s) {}
    };

    UnitTest(const char *name, void (*fcn)(void)) :
        name_(name),
        fcn_(fcn)
    {
        next_ = lll_unittest_list;
        lll_unittest_list = this;
    }

    void run(void) const {
        std::printf("Starting  testcase %s\n", name_);
        try {
            fcn_();
        } catch (const expect_error e) {
            std::printf("Testcase %s failed: %s\n",
                        name_,
                        e.what());
        }

        std::printf("Completed testcase %s\n", name_);
    }

    UnitTest *next() { return next_; }

    template <typename T>
    void expect_equal(int line, const T& a, const T& b) {

        if (!(a == b)) {
            std::stringstream buffer;
            buffer << "Line " << line << ", " << a << " != " << b;
            throw expect_error(buffer.str());
        }
    }
};

#define EXPECT_EQUAL(a,b) t.expect_equal(__LINE__, a, b)

} // test
} // lll

#endif 

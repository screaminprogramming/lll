/***********************************************************************
 * Low Latency Library: Test Program
 *
 * This is used just to test basic compilation and function of the LLL
 * library.  This should move to unit tests at some point
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include "lll/lll_unittest.h"

lll::unittest::UnitTest *lll::unittest::lll_unittest_list;

int main(int, char **) {
    auto test = lll::unittest::lll_unittest_list;
    while (test != nullptr) {
	test->run();
	test = test->next();
    }
}



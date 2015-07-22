/***********************************************************************
 * Low Latency Library: Test Program
 *
 * This is used just to test basic compilation and function of the LLL
 * library.  This should move to unit tests at some point
 *
 * Released under the MIT license. The LICENSE file should be included
 * in the top level of the source tree
 ***********************************************************************/
#include "lll/logger.h"

int main(int, char **) {
    char buffer[58]; 
    char output[256];
    
    memset(buffer, 0x00, sizeof(buffer));
   
    size_t logsize = lll_log(buffer, sizeof(buffer), "got [%s] [%u] [%s]", "XXXX", 2, "YYYY");

    if (!logsize)
        printf("Buffer too small\n");
    else
	lll::logging::logformat(buffer, output, sizeof(output));

    printf("[%lu] %s\n", logsize, output);
}



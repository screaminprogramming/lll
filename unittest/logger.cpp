#include "lll/logger.h"
#include "lll/lll_unittest.h"

namespace lll {
namespace unittest {

static UnitTest t(__FILE__,[](){
    char buffer[58]; 
    char output[256];
    
    memset(buffer, 0x00, sizeof(buffer));
    
    size_t logsize = lll_log(buffer, sizeof(buffer), "got [%s] [%u] [%s]", "XXXX", 2, "YYYY");
    
    if (!logsize)
        printf("Buffer too small\n");
    else
	lll::logging::logformat(buffer, output, sizeof(output));
    
    printf("[%lu] %s\n", logsize, output);
    
    });

}
}

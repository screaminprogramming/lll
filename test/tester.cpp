#include "logger.h"

static char *myprint(const char *format, ...)  {
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
    return NULL;
}

int main(int, char **) {
    char buffer[58]; 
    
    memset(buffer, 0x00, sizeof(buffer));
   
    lll::logging::logitem *li = lll_log(buffer, sizeof(buffer), "got [%s] [%u] [%s]", "XXXX", 2, "YYYY");

    if (!li)
        printf("Buffer too small\n");
    else
        li->format(myprint);
}



#include "../src/c/logger.h"

void my_func(){
    log_info("From my_func!");
}

int main(){
    log_init(LOG_DEBUG);
    log_debug("debug line. The code is %d", LOG_DEBUG);
    log_info("info line. The code is %d", LOG_INFO);
    log_warning("warning line. The code is %d", LOG_WARNING);
    log_error("error line. The code is %d", LOG_ERROR);
    my_func();
    return 0;
}
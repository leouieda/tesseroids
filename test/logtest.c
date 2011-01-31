#include "../src/c/logger.h"

// This line is necessary for now. Don't change the name of the variable!
LOGGER logger;

void my_func(){
    log_info("From my_func!\n");
}

int main(){
    log_init(LOG_DEBUG);
    log_debug("debug line. The code is %d\n", LOG_DEBUG);
    log_info("info line. The code is %d\n", LOG_INFO);
    log_warning("warning line. The code is %d\n", LOG_WARNING);
    log_error("error line. The code is %d\n", LOG_ERROR);
    return 0;
}
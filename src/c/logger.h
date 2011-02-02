/* *****************************************************************************
Copyright 2011 Leonardo Uieda

Tesseroids is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Tesseroids is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tesseroids.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************** */

/** \file
Functions to set up logging.

Example:

\verbatim
#include "logger.h"

void my_func(){
    log_info("From my_func!\n");
}

int main(){
    log_init(LOG_DEBUG);
    log_debug("debug line. The code is %d", LOG_DEBUG);
    log_info("info line. The code is %d", LOG_INFO);
    log_warning("warning line. The code is %d", LOG_WARNING);
    log_error("error line. The code is %d", LOG_ERROR);
    return 0;
}
\endverbatim

Will print:

\verbatim
DEBUG: debug line. The code is 0
info line. The code is 1
WARNING: warning line. The code is 2
ERROR: error line. The code is 3
\endverbatim

If function log_init() is not called than logging is disabled and no messages
will be printed to stderr.

@author Leonardo Uieda
@date 31 Jan 2011
*/

#ifndef _TESSEROIDS_LOGGER_H_
#define _TESSEROIDS_LOGGER_H_

/* Needed for definition of FILE */
#include <stdio.h>


/** Logging level for debug messages */
#define LOG_DEBUG   0
/** Logging level for general information */
#define LOG_INFO    1
/** Logging level for warning messages */
#define LOG_WARNING 2
/** Logging level for error messages */
#define LOG_ERROR   3


/** Keep the information on the global logger */
typedef struct logger_struct
{
    int level; /**< level of logging */
    int filelogging; /**< flag to know wether loggint to a file is enabled */
    int file_level; /**< logging level for the file */
    FILE *logfile; /**< file to log to */

} LOGGER;


/** Global logger struct. Only declare in the main program! */
extern LOGGER logger;


/** Setup logging to stderr.

@param level level of logging to be made. Can be one of:
    - LOG_DEBUG
    - LOG_INFO
    - LOG_WARNING
    - LOG_ERROR
*/
extern void log_init(int level);


/** Set logging to a file.

@param logfile FILE pointer to the already open file to log to.
@param level level of logging to be made to the file. Can be one of:
    - LOG_DEBUG
    - LOG_INFO
    - LOG_WARNING
    - LOG_ERROR
*/
extern void log_tofile(FILE *logfile, int level);


/** Log a message at debug level.

Pass parameters in the same format as printf()

Prints a newline at the end.
*/
extern void log_debug(const char *fmt, ...);


/** Log a message at info level.

Pass parameters in the same format as printf()

Does not print "INFO: " in front of the message when logging

Prints a newline at the end.
*/
extern void log_info(const char *fmt, ...);


/** Log a message at warning level.

Pass parameters in the same format as printf()

Prints a newline at the end.
*/
extern void log_warning(const char *fmt, ...);


/** Log a message at error level.

Pass parameters in the same format as printf()

Prints a newline at the end.
*/
extern void log_error(const char *fmt, ...);


#endif
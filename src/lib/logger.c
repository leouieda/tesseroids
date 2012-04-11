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

@author Leonardo Uieda
@date 31 Jan 2011
*/


#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"

/* Initialize the logger so that it doesn't print by default */
LOGGER logger = {.level = 100,
                 .file_level = 100,
                 .filelogging = 0,
                 .logfile = NULL};


/* Setup logging to stderr.*/
void log_init(int level)
{
    logger.level = level;
}


/* Set logging to a file. */
void log_tofile(FILE *logfile, int level)
{
    logger.filelogging = 1;
    logger.logfile = logfile;
    logger.file_level = level;
}


/* Log a message at debug level */
void log_debug(const char *fmt, ...)
{
    char msg[10000];
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);
    
    if(logger.level <= LOG_DEBUG)
    {
        fprintf(stderr, "DEBUG: %s\n", msg);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_DEBUG)
    {
        fprintf(logger.logfile, "DEBUG: %s\n", msg);
    }
}


/* Log a message at info level */
void log_info(const char *fmt, ...)
{
    char msg[10000];
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);
    
    if(logger.level <= LOG_INFO)
    {
        fprintf(stderr, "%s\n", msg);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_INFO)
    {
        fprintf(logger.logfile, "%s\n", msg);
    }
}


/* Log a message at warning level */
void log_warning(const char *fmt, ...)
{
    char msg[10000];
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);
    
    if(logger.level <= LOG_WARNING)
    {
        fprintf(stderr, "WARNING: %s\n", msg);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_WARNING)
    {
        fprintf(logger.logfile, "WARNING: %s\n", msg);
    }
}


/* Log a message at error level */
void log_error(const char *fmt, ...)
{
    char msg[10000];
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);
    
    if(logger.level <= LOG_ERROR)
    {
        fprintf(stderr, "ERROR: %s\n", msg);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_ERROR)
    {
        fprintf(logger.logfile, "ERROR: %s\n", msg);
    }
}

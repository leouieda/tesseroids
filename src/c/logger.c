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

/*
Functions to set up logging.

@author Leonardo Uieda
@date 31 Jan 2011
*/


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
    logger.filelogging = 0;
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
    if(logger.level <= LOG_DEBUG)
    {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "DEBUG: ");
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_DEBUG)
    {
        va_list args;
        va_start(args, fmt);
        fprintf(logger.logfile, "DEBUG: ");
        vfprintf(logger.logfile, fmt, args);
        va_end(args);
    }
}


/* Log a message at info level */
void log_info(const char *fmt, ...)
{
    
    if(logger.level <= LOG_INFO)
    {
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_INFO)
    {
        va_list args;
        va_start(args, fmt);
        vfprintf(logger.logfile, fmt, args);
        va_end(args);
    }
}


/* Log a message at warning level */
void log_warning(const char *fmt, ...)
{
    if(logger.level <= LOG_WARNING)
    {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "WARNING: ");
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_WARNING)
    {
        va_list args;
        va_start(args, fmt);
        fprintf(logger.logfile, "WARNING: ");
        vfprintf(logger.logfile, fmt, args);
        va_end(args);
    }
}


/* Log a message at error level */
void log_error(const char *fmt, ...)
{
    if(logger.level <= LOG_ERROR)
    {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "ERROR: ");
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
    
    if(logger.filelogging && logger.file_level <= LOG_ERROR)
    {
        va_list args;
        va_start(args, fmt);
        fprintf(logger.logfile, "ERROR: ");
        vfprintf(logger.logfile, fmt, args);
        va_end(args);
    }
}
/*
Hold the version number of the project.
*/

#include <stdio.h>
#include "version.h"

/* Current project version number */
const char tesseroids_version[] = "1.1.1";

/* Print version and license information */
void print_version(const char *progname)
{
    printf("%s (Tesseroids project) %s\n\n", progname, tesseroids_version);
    printf("Copyright (C) 2011-2014, Leonardo Uieda.\n");
    printf("This software is distributed under the terms of the BSD License:\n");
    printf("<http://tesseroids.readthedocs.org/en/latest/license.html>\n");
    printf("This is free software: ");
    printf("you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n\n");
    printf("Developed by Leonardo Uieda.\n");
}

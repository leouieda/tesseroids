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
Hold the version number of the project.

@author Leonardo Uieda
@date 01 Feb 2011
*/

#ifndef _TESSEROIDS_VERSION_H_
#define _TESSEROIDS_VERSION_H_

/** Current project version number */
extern const char tesseroids_version[];


/** Print version and license information

@param progname name of the program
*/
extern void print_version(const char *progname);

#endif
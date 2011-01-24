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

/* *****************************************************************************
This module contains functions for implementing a Gauss-Legendre Quadrature
numerical integration. To do this, use the glq_nodes function to compute the
discretization points for the function you want to integrate, next use the
glq_weights to calculate the weighting coefficients. You will need to implement
the integration loop(s) yourself but this allows for fast integration of any
function.

Author: Leonardo Uieda
Date: 24 Jan 2011
***************************************************************************** */

#include <math.h>
#include "glq.h"

/* Calculates the GLQ nodes using glq_next_root. */
int glq_nodes(int order, double *nodes)
{
    return 0;
}

/* Put the GLQ nodes to the integration limits IN PLACE. */
int glq_scale_nodes(double lower, double upper, int order, double *nodes)
{
    return 0;
}

/* Calculate the next Legendre polynomial root given the previous root found. */
int glq_next_root(double initial, int root_index, double *next_root)
{
    return 0;
}

/* Calculates the weighting coefficients for the GLQ integration. */
int glq_weights(int order, double *nodes, double *weights)
{
    return 0;
}

#endif
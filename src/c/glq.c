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

#include <stdlib.h>
#include <math.h>
#include "glq.h"

/* Calculates the GLQ nodes using glq_next_root. */
int glq_nodes(int order, double *nodes)
{
    if(order < 2)
    {
        return 1;
    }
    if(nodes == NULL)
    {
        return 2;
    }

    register int i;
    int rc = 0;
    double initial;

    for(i = 0; i < order; i++)
    {
        initial = cos(GLQ_PI*(order - i - 0.25)/(order + 0.5));
        
        if(glq_next_root(initial, i, order, nodes) == 3)
        {
            rc = 3;
        }
    }
    return rc;
}


/* Put the GLQ nodes to the integration limits IN PLACE. */
int glq_scale_nodes(double lower, double upper, int order, double *nodes)
{
    if(order < 2)
    {
        return 1;
    }
    if(nodes == NULL)
    {
        return 2;
    }

    /* Only calculate once to optimize the code */
    double tmpplus = 0.5*(upper + lower), tmpminus = 0.5*(upper - lower);
    register int i;
    
    for(i = 0; i < order; i++)
    {
        nodes[i] = tmpminus*nodes[i] + tmpplus;
    }

    return 0;
}


/* Calculate the next Legendre polynomial root given the previous root found. */
int glq_next_root(double initial, int root_index, int order, double *roots)
{
    
    if(order < 2)
    {
        return 1;
    }
    if(root_index < 0 || root_index >= order)
    {
        return 2;
    }

    double x1, x0, pn, pn_2, pn_1, pn_line, sum;
    int it = 0;
    register int n; 

    x1 = initial;
    do
    {
        x0 = x1;

        /* Calculate Pn(x0) */
        /* Starting from P0(x) and P1(x), */
        /* find the others using the recursive relation: */
        /*     Pn(x)=(2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n   */
        pn_1 = 1.;   /* This is Po(x) */
        pn = x0;    /* and this P1(x) */
        for(n = 2; n <= order; n++)
        {
            pn_2 = pn_1;
            pn_1 = pn;
            pn = ( ((2*n - 1)*x0*pn_1) - ((n - 1)*pn_2) )/n;
        }

        /* Now calculate Pn'(x0) using another recursive relation: */
        /*     Pn'(x)=n(xPn(x)-Pn_1(x))/(x*x-1)                    */
        pn_line = order*(x0*pn - pn_1)/(x0*x0 - 1);

        /* Sum the roots found so far */
        for(n = 0, sum = 0; n < root_index; n++)
        {
            sum += 1./(x0 - roots[n]);
        }

        /* Update the estimate for the root */
        x1 = x0 - (double)pn/(pn_line - pn*sum);

    } while(GLQ_ABS(x1 - x0) > GLQ_MAXERROR && ++it <= GLQ_MAXIT);

    roots[root_index] = x1;

    /* Tell the user if stagnation occurred */
    if(it > GLQ_MAXIT)
    {
        return 3;
    }

    return 0;
}


/* Calculates the weighting coefficients for the GLQ integration. */
int glq_weights(int order, double *nodes, double *weights)
{
    if(order < 2)
    {
        return 1;
    }
    if(nodes == NULL)
    {
        return 2;
    }
    if(weights == NULL)
    {
        return 3;
    }

    register int i, n;
    double xi, pn, pn_2, pn_1, pn_line; 

    for(i = 0; i < order; i++){

        xi = nodes[i];
        
        /* Find Pn'(xi) with the recursive relation to find Pn and Pn-1: */
        /*   Pn(x)=(2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n   */
        /* Then use:   Pn'(x)=n(xPn(x)-Pn_1(x))/(x*x-1) */

        /* Find Pn and Pn-1 stating from P0 and P1 */
        pn_1 = 1;   /* This is Po(x) */
        pn = xi;    /* and this P1(x) */
        for(n = 2; n <= order; n++)
        {
            pn_2 = pn_1;
            pn_1 = pn;
            pn = ((2*n - 1)*xi*pn_1 - (n - 1)*pn_2)/n;
        }

        pn_line = order*(xi*pn - pn_1)/(xi*xi - 1.);

        /* ith weight is: wi = 2/(1 - xi^2)(Pn'(xi)^2) */
        weights[i] = 2./((1 - xi*xi)*pn_line*pn_line);
    }
    
    return 0;
}
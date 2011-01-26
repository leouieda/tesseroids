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

#ifndef _GLQ_H_
#define _GLQ_H_

/* Max iterations of the root-finder algorithm */
#define GLQ_MAXIT 1000
/* Max error allowed for the root-finder algorithm */
#define GLQ_MAXERROR 0.000000000000001

#define GLQ_ABS(x) ((x) < 0 ? -1*(x) : (x))

/* Calculates the GLQ nodes using glq_next_root. Nodes will be in the [-1,1]
interval. To convert them to the integration limits use glq_scale_nodes.

Parameters:
    * int order: order of the quadrature, ie how many nodes. Must be >= 2.
    * double *nodes: pre-allocated array to return the nodes.

Returns:
    * 0: if everything went OK
    * 1: if invalid order
    * 2: if NULL pointer for nodes
    * 3: if number of maximum iterations was reached when calculating the root.
         This usually means that the desired accuracy was not achieved. Default
         desired accuracy is GLQ_MAXERROR. Default maximum iterations is
         GLQ_MAXIT.
*/
extern int glq_nodes(int order, double *nodes);

/* Put the GLQ nodes to the integration limits IN PLACE.

Parameters:
    * double lower: lower integration limit
    * double upper: upper integration limit
    * int order: order of the GLQ, ie how many nodes
    * double *nodes: array with the GLQ nodes in [-1,1] interval. WARNING:The
        scaled nodes will be returned overwritting this array!

Returns:
    * 0: if everything went OK
    * 1: if invalid order
    * 2: if NULL pointer for nodes
*/
extern int glq_scale_nodes(double lower, double upper, int order,
                           double *nodes);
                           
/* Calculate the next Legendre polynomial root given the previous root found.
Uses the root-finder algorithm of:
  Barrera-Figueroa, V., Sosa-Pedroza, J. and López-Bonilla, J., 2006, 
  "Multiple root finder algorithm for Legendre and Chebyshev polynomials via
  Newton's method", 2006, Annales mathematicae et Informaticae, 33, pp 3-13

Parameters:
    * double initial: initial estimate of the next root. I recommend the use of
        cos(PI*((order - i) - 0.25)/(order + 0.5)), where i is the index of the
        next root
    * int root_index: index of the next root, starting from 0. So the first root
        of the polynomial is root 0, the next is root 1 and so forth.
    * int order: order of the Legendre polynomial, ie number of roots.
    * double *roots: array with the roots found so far. Will return the next
        root in roots[root_index], so make sure to malloc enough space.

Returns:
    * 0: if everything went OK
    * 1: if order is not valid
    * 2: if root_index is not valid (negative)
    * 3: if number of maximum iterations was reached when calculating the root.
         This usually means that the desired accuracy was not achieved. Default
         desired accuracy is GLQ_MAXERROR. Default maximum iterations is
         GLQ_MAXIT.
*/
extern int glq_next_root(double initial, int root_index, int order,
                         double *roots);

/* Calculates the weighting coefficients for the GLQ integration.

Parameters:
    * int order: order of the quadrature, ie number of nodes and weights.
    * double *nodes: array containing the GLQ nodes calculated by glq_nodes.
        IMPORTANT: needs the nodes in [-1,1] interval! Scaled nodes will result
        in wrong weights!
    * double *weights: pre-allocated array to return the weights

Returns:
    * 0: if everything went OK
    * 1: if order is not valid
    * 2: if nodes is a NULL pointer
    * 3: if weights is a NULL pointer
*/
extern int glq_weights(int order, double *nodes, double *weights);

#endif
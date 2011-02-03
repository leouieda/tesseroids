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
Functions for implementing a Gauss-Legendre Quadrature numerical integration.

\f[
\int_a^b f(x) dx \approx \frac{b-a}{2} \displaystyle\sum_{i=0}^{N-1} w_i f(x_i)
\f]

\f$ N \f$ is the order of the quadrature.

Usage example:

To integrate the cossine function from 0 to 90 degrees

@verbatim
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "src/c/glq.h"

int main(){
    // Create a new glq structure
    GLQ *glq;
    double result = 0, a = 0, b = 0.5*3.14;
    int i;

    glq = glq_new(5, a, b);

    if(glq == NULL){
        printf("malloc error");
        return 1;
    }

    // Calculate the integral
    for(i = 0; i < glq->order; i++)
        result += glq->weights[i]*cos(glq->nodes[i]);

    // Need to multiply by a scale factor of the integration limits
    result *= 0.5*(b - a);

    printf("Integral of cossine from 0 to 90 degrees = %lf\n", result);
    return 0;
}
@endverbatim

\todo Change defines to variables so that they can be altered using config file

@author Leonardo Uieda
@date 24 Jan 2011
*/

#ifndef _TESSEROIDS_GLQ_H_
#define _TESSEROIDS_GLQ_H_


/** Max iterations of the root-finder algorithm */
#define GLQ_MAXIT 1000

/** Max error allowed for the root-finder algorithm */
#define GLQ_MAXERROR 0.000000000000001

/** Compute the absolute value of x */
#define GLQ_ABS(x) ((x) < 0 ? -1*(x) : (x))


/** Store the nodes and weights needed for a GLQ integration */
typedef struct glq_struct
{
    int order; /**< order of the quadrature, ie number of nodes */
    double *nodes; /**< abscissas or discretization points of the quadrature */
    double *weights; /**< weighting coefficients of the quadrature */
    double *nodes_unscaled; /**< nodes in [-1,1] interval */
} GLQ;


/** Make a new GLQ structure and set all the parameters needed

<b>WARNING</b>: Don't forget to free the memory malloced by this function using
glq_free()!

Prints error and warning messages using the logging.h module.

@param order order of the quadrature, ie number of nodes
@param lower lower integration limit
@param upper upper integration limit

@return GLQ data structure with the nodes and weights calculated. NULL if there
    was an error with allocation.
*/
extern GLQ * glq_new(int order, double lower, double upper);


/** Free the memory allocated to make a GLQ structure

@param glq pointer to the allocated memory
*/
extern void glq_free(GLQ *glq);


/** Put the GLQ nodes to the integration limits <b>IN PLACE</b>.

Will replace the values of glq.nodes with ones in the specified integration
limits.

In case the GLQ structure was created with glq_new(), the integration limits can
be reset using this function. 

@param lower lower integration limit
@param upper upper integration limit
@param glq pointer to a GLQ structure created with glq_new() and with all
           necessary memory allocated

@return Return code:
    - 0: if everything went OK
    - 1: if invalid order
    - 2: if NULL pointer for nodes or nodes_unscaled
*/
extern int glq_set_limits(double lower, double upper, GLQ *glq);


/** Calculates the GLQ nodes using glq_next_root.

Nodes will be in the [-1,1] interval. To convert them to the integration limits
use glq_scale_nodes

@param order order of the quadrature, ie how many nodes. Must be >= 2.
@param nodes pre-allocated array to return the nodes.

@return Return code:
    - 0: if everything went OK
    - 1: if invalid order
    - 2: if NULL pointer for nodes
    - 3: if number of maximum iterations was reached when calculating the root.
         This usually means that the desired accuracy was not achieved. Default
         desired accuracy is GLQ_MAXERROR. Default maximum iterations is
         GLQ_MAXIT.
*/
extern int glq_nodes(int order, double *nodes);

                           
/** Calculate the next Legendre polynomial root given the previous root found.

Uses the root-finder algorithm of:

  Barrera-Figueroa, V., Sosa-Pedroza, J. and López-Bonilla, J., 2006, 
  "Multiple root finder algorithm for Legendre and Chebyshev polynomials via
  Newton's method", 2006, Annales mathematicae et Informaticae, 33, pp 3-13

@param initial initial estimate of the next root. I recommend the use of
               \f$ \cos\left(\pi\frac{(N - i - 0.25)}{N + 0.5}\right) \f$,
               where \f$ i \f$ is the index of the desired root
@param root_index index of the desired root, starting from 0
@param order order of the Legendre polynomial, ie number of roots.
@param roots array with the roots found so far. Will return the next root in
             roots[root_index], so make sure to malloc enough space.

@return Return code:
    - 0: if everything went OK
    - 1: if order is not valid
    - 2: if root_index is not valid (negative)
    - 3: if number of maximum iterations was reached when calculating the root.
         This usually means that the desired accuracy was not achieved. Default
         desired accuracy is GLQ_MAXERROR. Default maximum iterations is
         GLQ_MAXIT.
*/
extern int glq_next_root(double initial, int root_index, int order,
                         double *roots);

                         
/** Calculates the weighting coefficients for the GLQ integration.

@param order order of the quadrature, ie number of nodes and weights.
@param nodes array containing the GLQ nodes calculated by glq_nodes.
             <b>IMPORTANT</b>: needs the nodes in [-1,1] interval! Scaled nodes
             will result in wrong weights!
@param weights pre-allocated array to return the weights

@return Return code:
    - 0: if everything went OK
    - 1: if order is not valid
    - 2: if nodes is a NULL pointer
    - 3: if weights is a NULL pointer
*/
extern int glq_weights(int order, double *nodes, double *weights);

#endif
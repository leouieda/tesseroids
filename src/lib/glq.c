/*
Functions for implementing a Gauss-Legendre Quadrature numerical integration.
*/


#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "logger.h"
#include "glq.h"


/** \var GLQ_MAXIT
Max iterations of the root-finder algorithm */
const int GLQ_MAXIT = 1000;


/** \var GLQ_MAXERROR
Max error allowed for the root-finder algorithm */
const double GLQ_MAXERROR = 0.000000000000001;


/* Make a new GLQ structure and set all the parameters needed */
GLQ * glq_new(int order, double lower, double upper)
{
    GLQ *glq;
    int rc;

    glq = (GLQ *)malloc(sizeof(GLQ));
    if(glq == NULL)
    {
        return NULL;
    }
    glq->order = order;
    glq->nodes = (double *)malloc(sizeof(double)*order);
    if(glq->nodes == NULL)
    {
        free(glq);
        return NULL;
    }
    glq->nodes_unscaled = (double *)malloc(sizeof(double)*order);
    if(glq->nodes_unscaled == NULL)
    {
        free(glq);
        free(glq->nodes);
        return NULL;
    }
    glq->weights = (double *)malloc(sizeof(double)*order);
    if(glq->weights == NULL)
    {
        free(glq);
        free(glq->nodes);
        free(glq->nodes_unscaled);
        return NULL;
    }
    rc = glq_nodes(order, glq->nodes_unscaled);    
    if(rc != 0 && rc != 3)
    {
        switch(rc)
        {
            case 1:
                log_error("glq_nodes invalid GLQ order %d. Should be >= 2.",
                          order);
                break;
            case 2:
                log_error("glq_nodes NULL pointer for nodes");
                break;
            default:
                log_error("glq_nodes unknown error code %g", rc);
                break;
        }
        glq_free(glq);
        return NULL;
    }
    else if(rc == 3)
    {
        log_warning("glq_nodes max iterations reached in root finder");
        log_warning("nodes might not have desired accuracy %g", GLQ_MAXERROR);
    }
    rc = glq_weights(order, glq->nodes_unscaled, glq->weights);
    if(rc != 0)
    {
        switch(rc)
        {
            case 1:
                log_error("glq_weights invalid GLQ order %d. Should be >= 2.",
                          order);
                break;
            case 2:
                log_error("glq_weights NULL pointer for nodes");
                break;
            case 3:
                log_error("glq_weights NULL pointer for weights");
                break;
            default:
                log_error("glq_weights unknown error code %d\n", rc);
                break;
        }
        glq_free(glq);
        return NULL;
    }
    if(glq_set_limits(lower, upper, glq) != 0)
    {
        glq_free(glq);
        return NULL;
    }    
    return glq;
}


/* Free the memory allocated to make a GLQ structure */
void glq_free(GLQ *glq)
{
    free(glq->nodes);
    free(glq->nodes_unscaled);
    free(glq->weights);
    free(glq);
}


/* Calculates the GLQ nodes using glq_next_root. */
int glq_nodes(int order, double *nodes)
{
    register int i;
    int rc = 0;
    double initial;
    
    if(order < 2)
    {
        return 1;
    }
    if(nodes == NULL)
    {
        return 2;
    }
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*(order - i - 0.25)/(order + 0.5));
        if(glq_next_root(initial, i, order, nodes) == 3)
        {
            rc = 3;
        }
    }
    return rc;
}


/* Put the GLQ nodes to the integration limits IN PLACE. */
int glq_set_limits(double lower, double upper, GLQ *glq)
{
    /* Only calculate once to optimize the code */
    double tmpplus = 0.5*(upper + lower), tmpminus = 0.5*(upper - lower);
    register int i;
    
    if(glq->order < 2)
    {
        return 1;
    }
    if(glq->nodes == NULL)
    {
        return 2;
    }
    if(glq->nodes_unscaled == NULL)
    {
        return 2;
    }    
    for(i = 0; i < glq->order; i++)
    {
        glq->nodes[i] = tmpminus*glq->nodes_unscaled[i] + tmpplus;
    }
    return 0;
}


/* Calculate the next Legendre polynomial root given the previous root found. */
int glq_next_root(double initial, int root_index, int order, double *roots)
{
    double x1, x0, pn, pn_2, pn_1, pn_line, sum;
    int it = 0;
    register int n; 
    
    if(order < 2)
    {
        return 1;
    }
    if(root_index < 0 || root_index >= order)
    {
        return 2;
    }
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
        
    /** Compute the absolute value of x */
    #define GLQ_ABS(x) ((x) < 0 ? -1*(x) : (x))
    } while(GLQ_ABS(x1 - x0) > GLQ_MAXERROR && ++it <= GLQ_MAXIT);
    #undef GLQ_ABS
    
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
    register int i, n;
    double xi, pn, pn_2, pn_1, pn_line;
    
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

/* 
Unit tests for GLQ functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "minunit.h"
#include "../src/lib/glq.h"
#include "../src/lib/constants.h"

/* Test data taken from:
    http://mathworld.wolfram.com/Legendre-GaussQuadrature.html */
double o2roots[2] = {-0.577350269, 0.577350269},
       o3roots[3] = {-0.774596669, 0., 0.774596669},
       o4roots[4] = {-0.861136312, -0.339981044, 0.339981044, 0.861136312},
       o5roots[5] = {-0.906179846, -0.53846931, 0., 0.53846931, 0.906179846},
       o19roots[19] = {-0.992406843843584350,
                       -0.960208152134830020,
                       -0.903155903614817900,
                       -0.822714656537142820,
                       -0.720966177335229390,
                       -0.600545304661680990,
                       -0.464570741375960940,
                       -0.316564099963629830,
                       -0.160358645640225370,
                        0.000000000000000000,
                        0.160358645640225370,
                        0.316564099963629830,
                        0.464570741375960940,
                        0.600545304661680990,
                        0.720966177335229390,
                        0.822714656537142820,
                        0.903155903614817900,
                        0.960208152134830020,
                        0.992406843843584350};

double o2weights[2] = {1., 1.},
       o3weights[3] = {0.555555556, 0.888888889, 0.555555556},
       o4weights[4] = {0.347854845, 0.652145155, 0.652145155, 0.347854845},
       o5weights[5] = {0.236926885, 0.47862867, 0.568888889, 0.47862867,
            0.236926885};

/* To store fail messages */
char msg[1000];

/* UNIT TESTS */

static char * test_glq_next_root_fail()
{
    double roots[10];
    int i, order, rc;

    /* Test order fail */
    i = 1;
    order = -1;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(order %d) return code %d, expected 1", order, rc);
    mu_assert(rc == 1, msg);
    
    order = 0;
    rc = glq_next_root(-0.1, i, order, roots);
    sprintf(msg, "(order %d) return code %d, expected 1", order, rc);
    mu_assert(rc == 1, msg);

    order = 1;
    rc = glq_next_root(1.1, i, order, roots);
    sprintf(msg, "(order %d) return code %d, expected 1", order, rc);
    mu_assert(rc == 1, msg);

    /* Test index fail */
    order = 5;
    i = -1;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(index %d, order %d) return code %d, expected 2", order, i,
            rc);
    mu_assert(rc == 2, msg);
    
    i = 5;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(index %d, order %d) return code %d, expected 2", order, i,
            rc);
    mu_assert(rc == 2, msg);

    i = 10;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(index %d, order %d) return code %d, expected 2", order, i,
            rc);
    mu_assert(rc == 2, msg);
    
    return 0;
}


static char * test_glq_next_root()
{
    double prec = pow(10, -9), root[19], initial;
    int rc, i, order;
    
    /* Test order 2 */
    order = 2;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));
        
        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o2roots[i], root[i]);
        mu_assert_almost_equals(root[i], o2roots[i], prec, msg);
    }
    
    /* Test order 3 */
    order = 3;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o3roots[i], root[i]);
        mu_assert_almost_equals(root[i], o3roots[i], prec, msg);
    }

    /* Test order 4 */
    order = 4;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o4roots[i], root[i]);
        mu_assert_almost_equals(root[i], o4roots[i], prec, msg);
    }

    /* Test order 5 */
    order = 5;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o5roots[i], root[i]);
        mu_assert_almost_equals(root[i], o5roots[i], prec, msg);
    }

    /* Test order 19 */
    order = 19;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o19roots[i], root[i]);
        mu_assert_almost_equals(root[i], o19roots[i], prec, msg);
    }

    return 0;
}


static char * test_glq_weights()
{
    double prec = pow(10, -9), weights[5];
    int rc, i, order;

    /* Test order 2 */
    order = 2;
    
    rc = glq_weights(order, o2roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o2weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o2weights[i], prec, msg);
    }
    
    /* Test order 3 */
    order = 3;

    rc = glq_weights(order, o3roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o3weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o3weights[i], prec, msg);
    }

    /* Test order 4 */
    order = 4;

    rc = glq_weights(order, o4roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o4weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o4weights[i], prec, msg);
    }

    /* Test order 5 */
    order = 5;

    rc = glq_weights(order, o5roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o5weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o5weights[i], prec, msg);
    }

    return 0;
}


static char * test_glq_nodes()
{
    double prec = pow(10, -9), nodes[19];
    int rc, i, order;

    /* Test order 2 */
    order = 2;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o2roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o2roots[i], prec, msg);
    }

    /* Test order 3 */
    order = 3;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o3roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o3roots[i], prec, msg);
    }

    /* Test order 4 */
    order = 4;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o4roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o4roots[i], prec, msg);
    }

    /* Test order 5 */
    order = 5;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o5roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o5roots[i], prec, msg);
    }

    /* Test order 19 */
    order = 19;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o19roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o19roots[i], prec, msg);
    }

    return 0;
}


static char * test_glq_set_limits()
{
    double prec = pow(10, -9), unscaled[5], scaled[5], a, b, correct;
    int rc, i;
    GLQ glq;
    
    glq.nodes_unscaled = unscaled;
    glq.nodes = scaled;

    glq.order = 2;
    a = -2.54;
    b = 14.9;
    mu_arraycp(o2roots, glq.nodes_unscaled, glq.order);
    
    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);
    
    for(i = 0; i < glq.order; i++)
    {
        correct = 8.72*o2roots[i] + 6.18;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    glq.order = 3;
    a = 125.6; 
    b = 234.84;
    mu_arraycp(o3roots, glq.nodes_unscaled, glq.order);

    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < glq.order; i++)
    {
        correct = 54.62*o3roots[i] + 180.22;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    glq.order = 4;
    a = 3.5;
    b = -12.4;
    mu_arraycp(o4roots, glq.nodes_unscaled, glq.order);

    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < glq.order; i++)
    {
        correct = -7.95*o4roots[i] - 4.45;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    glq.order = 5;
    a = 0.0;
    b = 0.0;
    mu_arraycp(o5roots, glq.nodes_unscaled, glq.order);

    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < glq.order; i++)
    {
        correct = 0.0;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    return 0;
}


static char * test_glq_intcos()
{
    double result, expected;
    double angles[6];
    int i, t, orders[6] = {2, 3, 5, 8, 15, 25};
    GLQ *glq;

    angles[0] = PI*0.1;
    angles[1] = PI;
    angles[2] = PI*1.2;
    angles[3] = PI*1.9;
    angles[4] = PI*4.3;
    angles[5] = PI*6.9;
    
    for(t = 0; t < 6; t++)
    {
        glq = glq_new(orders[t], 0., angles[t]);

        if(glq == NULL)
        {
            sprintf(msg,
                "(order %d, angle %g) failed to create new GLQ struct",
                orders[t], angles[t]);
            mu_assert(0, msg);
        }

        for(i = 0, result = 0; i < orders[t]; i++)
        {
            result += glq->weights[i]*cos(glq->nodes[i]);
        }
        result *= 0.5*angles[t];

        expected = sin(angles[t]);

        glq_free(glq);

        sprintf(msg, "(order %d, angle %g) expected %f, got %f", orders[t],
                angles[t], expected, result);
        mu_assert_almost_equals(result, expected, pow(10, -5), msg);
    }

    return 0;
}


void glq_run_all()
{
    mu_run_test(test_glq_next_root_fail,
                "glq_next_root returns correct fail code");
    mu_run_test(test_glq_next_root, "glq_next_root produces correct results");
    mu_run_test(test_glq_nodes, "glq_nodes produces correct results");
    mu_run_test(test_glq_set_limits,
                "glq_set_limits produces correct results");
    mu_run_test(test_glq_weights, "glq_weights produces correct results");
    mu_run_test(test_glq_intcos,
                "glq cossine integration produces correct results");
}

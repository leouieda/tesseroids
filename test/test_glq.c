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
Unit tests for GLQ functions.

Author: Leonardo Uieda
Date: 24 Jan 2011
***************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "minunit.h"
#include "../src/c/glq.h"
#include "../src/c/constants.h"

/* Test data taken from:
    http://mathworld.wolfram.com/Legendre-GaussQuadrature.html */
double o2roots[2] = {-(double)sqrt(3.)/3., (double)sqrt(3.)/3.},
       o3roots[3] = {-(double)sqrt(15.)/5., 0., (double)sqrt(15.)/5.},
       o4roots[4] = {-(double)sqrt(525. + 70.*sqrt(30.))/35.,
                     -(double)sqrt(525. - 70.*sqrt(30.))/35.,
                     (double)sqrt(525. - 70.*sqrt(30.))/35.,
                     (double)sqrt(525. + 70.*sqrt(30.))/35.},
       o5roots[5] = {-(double)sqrt(245. + 14.*sqrt(70.))/21.,
                     -(double)sqrt(245. - 14.*sqrt(70.))/21., 0.,
                     (double)sqrt(245. - 14.*sqrt(70.))/21.,
                     (double)sqrt(245. + 14.*sqrt(70.))/21.},
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
       o3weights[3] = {(double)5./9., (double)8./9., (double)5./9.},
       o4weights[4] = {(double)(18. - sqrt(30.))/36.,
                       (double)(18. + sqrt(30.))/36.,
                       (double)(18. + sqrt(30.))/36.,
                       (double)(18. - sqrt(30.))/36.},
       o5weights[5] = {(double)(322. - 13*sqrt(70.))/900.,
                       (double)(322. + 13*sqrt(70.))/900.,
                       (double)128./225,
                       (double)(322. + 13*sqrt(70.))/900.,
                       (double)(322. - 13*sqrt(70.))/900.};

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
    double prec = pow(10, -15), root[19], initial;
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

        sprintf(msg, "(order %d, root %d) expected %.15lf got %.15lf", order, i,
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

        sprintf(msg, "(order %d, root %d) expected %.15lf got %.15lf", order, i,
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

        sprintf(msg, "(order %d, root %d) expected %.15lf got %.15lf", order, i,
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

        sprintf(msg, "(order %d, root %d) expected %.15lf got %.15lf", order, i,
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

        sprintf(msg, "(order %d, root %d) expected %.15lf got %.15lf", order, i,
                o19roots[i], root[i]);
        mu_assert_almost_equals(root[i], o19roots[i], prec, msg);
    }

    return 0;
}


static char * test_glq_weights()
{
    double prec = pow(10, -15), weights[5];
    int rc, i, order;

    /* Test order 2 */
    order = 2;
    
    rc = glq_weights(order, o2roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15lf got %.15lf", order,
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
        sprintf(msg, "(order %d, weight %d) expected %.15lf got %.15lf", order,
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
        sprintf(msg, "(order %d, weight %d) expected %.15lf got %.15lf", order,
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
        sprintf(msg, "(order %d, weight %d) expected %.15lf got %.15lf", order,
                i, o5weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o5weights[i], prec, msg);
    }

    return 0;
}


static char * test_glq_nodes()
{
    double prec = pow(10, -15), nodes[19];
    int rc, i, order;

    /* Test order 2 */
    order = 2;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15lf got %.15lf", order,
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
        sprintf(msg, "(order %d, node %d) expected %.15lf got %.15lf", order,
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
        sprintf(msg, "(order %d, node %d) expected %.15lf got %.15lf", order,
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
        sprintf(msg, "(order %d, node %d) expected %.15lf got %.15lf", order,
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
        sprintf(msg, "(order %d, node %d) expected %.15lf got %.15lf", order,
                i, o19roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o19roots[i], prec, msg);
    }

    return 0;
}


static char * test_glq_scale_nodes()
{
    double prec = pow(10, -15), scaled[5], a, b, correct;
    int order, rc, i;

    order = 2;
    a = -2.54;
    b = 14.9;
    mu_arraycp(o2roots, scaled, order);
    
    rc = glq_scale_nodes(a, b, order, scaled);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", order, a,
            b, rc);
    mu_assert(rc == 0, msg);
    
    for(i = 0; i < order; i++)
    {
        correct = 8.72*o2roots[i] + 6.18;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15lf, got %.15lf",
                order, i, a, b, correct, scaled[i]);
        mu_assert_almost_equals(scaled[i], correct, prec, msg);
    }

    order = 3;
    a = 125.6; 
    b = 234.84;
    mu_arraycp(o3roots, scaled, order);

    rc = glq_scale_nodes(a, b, order, scaled);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", order, a,
            b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        correct = 54.62*o3roots[i] + 180.22;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15lf, got %.15lf",
                order, i, a, b, correct, scaled[i]);
        mu_assert_almost_equals(scaled[i], correct, prec, msg);
    }

    order = 4;
    a = 3.5;
    b = -12.4;
    mu_arraycp(o4roots, scaled, order);

    rc = glq_scale_nodes(a, b, order, scaled);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", order, a,
            b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        correct = -7.95*o4roots[i] - 4.45;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15lf, got %.15lf",
                order, i, a, b, correct, scaled[i]);
        mu_assert_almost_equals(scaled[i], correct, prec, msg);
    }

    order = 5;
    a = 0.0;
    b = 0.0;
    mu_arraycp(o5roots, scaled, order);

    rc = glq_scale_nodes(a, b, order, scaled);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", order, a,
            b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        correct = 0.0;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15lf, got %.15lf",
                order, i, a, b, correct, scaled[i]);
        mu_assert_almost_equals(scaled[i], correct, prec, msg);
    }

    return 0;
}


static char * test_glq_intcos()
{
    double result, expected;
    double angles[6] = {PI*0.1, PI, PI*1.2,
                        PI*1.9, PI*4.3, PI*6.9};
    int i, t, orders[6] = {2, 3, 5, 8, 15, 25};
    GLQ *glq;
    
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

        sprintf(msg, "(order %d, angle %g) expected %lf, got %lf", orders[t],
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
    mu_run_test(test_glq_scale_nodes,
                "glq_scale_nodes produces correct results");
    mu_run_test(test_glq_weights, "glq_weights produces correct results");
    mu_run_test(test_glq_intcos,
                "glq cossine integration produces correct results");
}
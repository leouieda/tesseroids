/*
Functions that calculate the gravitational potential and its first and second
derivatives for the rectangular prism in spherical coordinates.
Uses the formulas in Nagy et al. (2000).

References
----------

* Nagy, D., Papp, G., Benedek, J. (2000): The gravitational potential and its
  derivatives for the prism. Journal of Geodesy, 74, 552–560.
*/


#ifndef _TESSEROIDS_GRAV_PRISM_SPH_H_
#define _TESSEROIDS_GRAV_PRISM_SPH_H_


/* Needed for definition of PRISM */
#include "geometry.h"

/* Transform spherical coordinates to local Cartesian coordinates of the prism

Parameters:

* lon, lat, r: spherical coordinates of the point.
* prism: a prism whose lon, lat, r values will be used as the origin of the
         local coordinate system.
* x, y, z: used to return the x, y, z Cartesian coordinates of the point.
*/
extern int global2local(double lon, double lat, double r, PRISM prism,
    double *x, double *y, double *z);


/* Rotate the g vector or the ggt from the prisms coordinate system to the local
system of the computation point.

Parameters:

* atprism: the 3 component gravity vector or 9 component gravity tensor
           calculated in the local coordinates of the prism.
* nc: number of columns in atprism. 1 if the vector, 3 if the tensor.
* prism: the prism used to calculate atprism.
* lon, lat, r: coordinates of the computation point.
* atpoint: used to return the 3 component gravity vector or 9 component gravity
           tensor calculated in the local coordinates of the computation point.
*/
extern int prism2point(double *atprism, int nc, PRISM prism, double lon,
    double lat, double r, double *atpoint);

/* Calculates the gravity gradient tensor caused by a prism.
 
Parameters:

* prism: the prism whose effect will be calculated.
* lonp, latp, rp: coordinates of the computation point.
* ggt: 6 element array used to return the gradient tensor. The order is:
       gxx, gxy, gxz, gyy, gyz, gzz
*/
extern int prism_ggt_sph(PRISM prism, double lonp, double latp, double rp,
    double *ggt);
   
/* Calculates the gravitational attraction caused by a prism. 
 
Parameters:

* prism: the prism whose effect will be calculated.
* lonp, latp, rp: coordinates of the computation point.
* gx, gy, gz: used to return the 3 components of the gravity vector
*/
extern int prism_g_sph(PRISM prism, double lonp, double latp, double rp,
    double *gx, double *gy, double *gz);
                  
/* Calculates the potential caused by a prism. 
 
Parameters:

* prism: the prism whose effect will be calculated.
* lonp, latp, rp: coordinates of the computation point.

Returns:

* the calculated potential
*/
extern double prism_pot_sph(PRISM prism, double lonp, double latp, double rp);
#endif

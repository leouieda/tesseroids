#ifndef _GYYCALCULATOR2D_H_
#define _GYYCALCULATOR2D_H_

/* ****************************************************************************************** */
/* ********************************** GyyCalculator2D *************************************** */
/* ****************************************************************************************** */
/* **************** Calculate the Gyy component of the gravity gradient tensor ************** */
/* ****************** using analytical integration in r and 2D GLQ ************************** */
/* ****************************************************************************************** */
/* ************************** Developed by Leonardo Uieda *********************************** */
/* ****************************************************************************************** */
/* ************************* Geophysics undergraduate at the ******************************** */
/* ********** Institute of Astronomy, Geophysics and Atmospheric Sciences of the ************ */
/* **************************** University of São Paulo ************************************* */
/* ****************************************************************************************** */
/* ************************************** 2008 ********************************************** */
/* ****************************************************************************************** */
/* *************************** Compiled in Windows with g++ ********************************* */
/* ********************* To be tested in Linux Ubuntu and Solaris *************************** */
/* ****************************************************************************************** */

#ifndef _FIELDCALCULATOR_H_
#include "FieldCalculator.h"
#endif

#ifndef _ABSCISSAS_H_
#include "Abscissas.h"
#endif

#ifndef _DATA_H_
#include "Data.h"
#endif

#ifndef _WEIGHTS_H_
#include "Weights.h"
#endif

#ifndef _MODELELEMENT_H_
#include "ModelElement.h"
#endif

#ifndef _EXCEPTIONS_H_
#include "exceptions.h"
#endif

#ifndef _MATH_H_
#include<math.h>
#endif

#ifndef L_MODULO
#define L_MODULO(a) (((a)>=0)?(a):((-1)*(a)))
#endif

#ifndef POS
#define POS(i,j,ncols) (((i)*(ncols))+(j))
#endif

class GyyCalculator2D: public FieldCalculator {
    
    /* PROTECTED PARTS */
    private:
        /* FIELDS */
        /* ************************************************************** */
        
        /* The Gauss-Legendre Quadrature order */
        int latOrder, lonOrder;
        
        /* Conversion factor from 1/s^2 to Eotvos */
        static double si2eotvos;
        
        /* ************************************************************** */
            
    /* PUBLIC PARTS */
    public:
        /* CONSTRUCTORS */
        /* ************************************************************** */
                
        GyyCalculator2D(){
            latOrder = 2;
            lonOrder = 2;          
            boundsSet = false;
            hightSet = false;
            dimSet = false;
        }
        
        GyyCalculator2D(int newLatO, int newLonO){
            latOrder = newLatO;
            lonOrder = newLonO;        
            boundsSet = false;
            hightSet = false;
            dimSet = false;
        }
        
        GyyCalculator2D(double newW, double newE, double newS, double newN, double newZ, int newNlon, int newNlat, int newLatO, int newLonO){            
            latOrder = newLatO;
            lonOrder = newLonO;        
            W = newW;
            E = newE;
            S = newS;
            N = newN;
            Z = newZ;
            nlon = newNlon;
            nlat = newNlat;
            n = nlon * nlat;        
            boundsSet = true;
            hightSet = true;
            dimSet = true;
        }
        
        /* ************************************************************** */


        /* METHODS */   
        /* ************************************************************** */

        
        /* Calculate the Gyy <spherical> component of the Gravity Gradient Tensor using analytical integration */
        /* in r and Gauss-Legendre Quadrature in lat and lon */
        Data* calculate(vector<ModelElement>& mod_vector) throw (GridParamNotSetException){
            
            
            /* Check if all params were set before trying to calculate the fields */
            if(!paramsSet()){
                GridParamNotSetException ex("Gyy <spherical> component of gravity gradient tensor <2D GLQ>");
                throw ex;
            }
            
            /* VARIABLES */
            /* ********************************************************* */
            
            /* The data grid that will contain the results */
            Data *grd = new Data[nlon*nlat];
            
            /* Convert the model to an array for faster manipulation */
            ModelElement *mod = &mod_vector[0];
            unsigned int modSize = mod_vector.size();
            unsigned int e;  /* Model element counter */
            
            /* The abscissas and weights of the GLQ */
            Abscissas absLat(latOrder),
                      absLon(lonOrder);                     
            Weights wLat(absLat),
                    wLon(absLon);
            /* Convert the abscissas and weights to arrays for faster manipulation */
            double *absLatA = absLat.toArray(),
                   *absLonA = absLon.toArray(),
                   *wLatA = wLat.toArray(),
                   *wLonA = wLon.toArray();
            
            /* GLQ stuff */
            register int i, j; /* GLQ indexes */ 
            double tmpWLat, /* Temps used to avoid using [] too much. this speeds up the program */
                   scaleFactor, /* The scale factor used because of the variable change made to tranform the the integration interval to [-1,1] */
                   res; /* Aux for the sum */
                   
            /* Model element temporary holders */
            double eW, eE, eS, eN, r2, r1; /* The model element bounds. Keep them here to avoid calling get*() all the time */
                              
            /* Grid stuff */
            unsigned int clat, clon; /* grid position counters */
            double phi, lamb, r = R + Z, /* Computation point coordinates (grid nodes). r is mean earth radius plus elevation */
                   latStep, lonStep, /* Interval between the grid nodes */ 
                   Wrad = deg2rad*W, /*Erad = deg2rad*E,*/ Srad = deg2rad*S/*, Nrad = deg2rad*N*/; /* The grid bounds in radians */
                               
            /* Aux for calculating KV */                   
            double lambl, phil, r_2,
                   r1_2,
                   r2_2, 
                   cosPhil,
                   cosPhi,                   
                   sinPhil,
                   sinPhi,
                   cosLambLambl,                 
                   cosPsi, 
                   cosPsiPhi,     
                   cosPsiLamb, 
                   cosPsiLamb_2,
                   cosPsiLambLamb,            
                   l1,
                   l2,
                   l1_2,
                   l2_2,
                   r1l1,
                   r2l2,
                   lntop,
                   lnbot,
                   cosPsi_2_1,
                   ln,
                   t1, t2, t3, t4, t5, t6, t7,
                   KVlamb_2, KVphi, KVr, KV;
           
            /* ********************************************************* */
            
            
            /* Initialize the data grid with zeros */
            lonStep = (E - W) / (nlon-1);
            latStep = (N - S) / (nlat-1);
            for(phi=S, clat=0; /*lat<=N && */clat<nlat; phi+=latStep, clat++){
                for(lamb=W, clon=0; /*lon<=E && */clon<nlon; lamb+=lonStep, clon++){
                    grd[POS(clat,clon,nlon)].set(lamb,phi,0);
                }
            }  
            
            /* Convert the intervals to radians */
            latStep = deg2rad*latStep;
            lonStep = deg2rad*lonStep;         
            
            
            /* Sum together the influence of each model element */
            for(e=0; e < modSize; e++){
               
                /* Keep the model element params */
                /* Using radians instead of degrees because the c math functions expect them to be so */
                /* Convert depth (z positive downward) to radius (using the mean earth radius) */
                /* This way the get*() functions are called only once */
                /* It may not seem like it, but this helps speed up the program */
                /* Function calls are much slower than = */
                eW = deg2rad*mod[e].getW();
                eE = deg2rad*mod[e].getE();
                eS = deg2rad*mod[e].getS();
                eN = deg2rad*mod[e].getN();
                r2 = R-mod[e].getTop();
                r1 = R-mod[e].getBottom();
                              
                /* Scale the abscissas for this model element */               
                absLat.scale(eS, eN);
                absLon.scale(eW, eE);
                         
                /* Calculate the scale factor */
                scaleFactor = (si2eotvos*(eN-eS)*(eE-eW)*mod[e].getDensity()*G) / 4;
                             
                /* Calculte the field caused by this element on the data grid */
                for(phi=Srad, clat=0; /*lat<=Nrad && */clat<nlat; phi+=latStep, clat++){
                    for(lamb=Wrad, clon=0; /*lon<=Erad && */clon<nlon; lamb+=lonStep, clon++){
                        
                        /* Do the GLQ */
                        res = 0;
                        for(i=0; i<latOrder; i++){
                            phil = absLatA[i];
                            tmpWLat = wLatA[i];
                            for(j=0; j<lonOrder; j++){     
                                /* Calculate KV */
                                lambl = absLonA[j];
                                r_2 = r*r;
                                r1_2 = r1*r1;
                                r2_2 = r2*r2; 
                                cosPhil = cos(phil);
                                cosPhi = cos(phi);                 
                                sinPhil = sin(phil);
                                sinPhi = sin(phi);
                                cosLambLambl = cos(lamb-lambl);                          
                                cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl;
                                cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl;
                                cosPsiLamb = (-1)*cosPhi*cosPhil*sin(lamb-lambl);
                                cosPsiLamb_2 = cosPsiLamb*cosPsiLamb;
                                cosPsiLambLamb = (-1)*cosPhi*cosPhil*cosLambLambl;
                                l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) );
                                l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) );
                                l1_2 = l1*l1;
                                l2_2 = l2*l2;
                                r1l1 = r1 / l1;
                                r2l2 = r2 / l2;
                                lntop = l2+r2-(r*cosPsi);
                                lnbot = l1+r1-(r*cosPsi);
                                cosPsi_2_1 = (3*cosPsi*cosPsi) - 1;
                                ln = log(L_MODULO( lntop / lnbot ) );
                                /* KVlamb^2 */
                                t1 = (r*r2l2*r2l2/l2)*(r*r2*cosPsiLamb_2 + l2_2*cosPsiLambLamb);
                                t2 = (r*r1l1*r1l1/l1)*(r*r1*cosPsiLamb_2 + l1_2*cosPsiLambLamb);
                                t3 = 3*r_2*cosPsiLamb_2*(2*(r1l1-r2l2) + r*cosPsi*((r1l1*r1l1/l1) - (r2l2*r2l2/l2)));
                                t4 = 3*r*cosPsiLambLamb*(l2 - l1  + r*cosPsi*(r1l1 - r2l2));
                                t5 = 6*r_2*ln*(cosPsiLamb_2 + cosPsiLambLamb*cosPsi);
                                t6 = r_2*r*(12*cosPsi*cosPsiLamb_2 + cosPsiLambLamb*cosPsi_2_1)*((r1+l1)/(l1*lnbot) - (r2+l2)/(l2*lntop));
                                t7 = r_2*r_2*cosPsiLamb_2*cosPsi_2_1*((r1*lnbot - (r1+l1)*(r1l1*lnbot + r1 + l1))/(l1*l1*lnbot*lnbot) - (r2*lntop - (r2+l2)*(r2l2*lntop + r2 + l2))/(l2*l2*lntop*lntop));
                                KVlamb_2 = 0.5*( t2 - t1 + t3 + t4 + t5 + t6 - t7 );
                                /* KVphi */ 
                                KVphi = (cosPsiPhi/2)*(r*r1*r1l1 - r*r2*r2l2 + 3*r*(l2 - l1 + r*cosPsi*(r1l1 - r2l2)) + 6*r_2*cosPsi*ln + r_2*r*cosPsi_2_1*((r1+l1)/(l1*lnbot) - (r2+l2)/(l2*lntop)));
                                /* KVr */
                                KVr = ( (r2*l2) - (r1*l1) + (3*r*cosPsi*(l2-l1)) + (r_2*cosPsi_2_1*ln) - ((r2l2*r2_2) + ((r1l1*r1_2))) ) / r;
                                
                                KV = cosPhil*((KVlamb_2 + (r*cosPhi*cosPhi*KVr) - (cosPhi*sinPhi*KVphi))/(r_2*cosPhi*cosPhi));
                                                                              
                                res += (tmpWLat*wLonA[j])*KV;                              
                            }
                        }
                        res *= scaleFactor;
                        
                        /* Now res contains the value of the integral at point (r,lat,lon) */
                        /* Add this to the data grid */ 
                        grd[POS(clat,clon,nlon)].addTo(res);
                    }
                }                   
            } 
            return grd;
        }
};

/* Conversion factor */
/* 1 /s^2 = 10^9 Eotvos */
double GyyCalculator2D::si2eotvos = 1000000000;

#endif

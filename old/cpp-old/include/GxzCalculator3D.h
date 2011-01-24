#ifndef _GXZCALCULATOR3D_H_
#define _GXZCALCULATOR3D_H_

/* ****************************************************************************************** */
/* ********************************** GxzCalculator3D *************************************** */
/* ****************************************************************************************** */
/* **************** Calculate the Gxz component of the gravity gradient tensor ************** */
/* ********************************** using 3D GLQ ****************************************** */
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

class GxzCalculator3D: public FieldCalculator {
    
    /* PROTECTED PARTS */
    private:
        /* FIELDS */
        /* ************************************************************** */
        
        /* The Gauss-Legendre Quadrature order */
        int latOrder, lonOrder, rOrder;
        
        /* Conversion factor from 1/s^2 to Eotvos */
        static double si2eotvos;
        
        /* ************************************************************** */
            
        
    /* PUBLIC PARTS */
    public:
        /* CONSTRUCTORS */
        /* ************************************************************** */
                
        GxzCalculator3D(){
            latOrder = 2;
            lonOrder = 2;
            rOrder = 2;
            boundsSet = false;
            hightSet = false;
            dimSet = false;
        }
        
        GxzCalculator3D(int newLatO, int newLonO, int newRO){
            latOrder = newLatO;
            lonOrder = newLonO;
            rOrder = newRO;
            boundsSet = false;
            hightSet = false;
            dimSet = false;
        }
        
        GxzCalculator3D(double newW, double newE, double newS, double newN, double newZ, int newNlon, int newNlat, int newLatO, int newLonO, int newRO){            
            latOrder = newLatO;
            lonOrder = newLonO;
            rOrder = newRO;
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


        /* Calculate the Gxz <spherical> component of the Gravity Gradient Tensor using a 3D Gauss-Legendre Quadrature */
        Data* calculate(vector<ModelElement>& mod_vector) throw (GridParamNotSetException){
            
            
            /* Check if all params were set before trying to calculate the fields */
            if(!paramsSet()){
                GridParamNotSetException ex("Gxz <spherical> component of gravity gradient tensor <3D GLQ>");
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
                      absLon(lonOrder),
                      absR(rOrder);
            Weights wLat(absLat),
                    wLon(absLon),
                    wR(absR);
            /* Convert the abscissas and weights to arrays for faster manipulation */
            double *absLatA = absLat.toArray(),
                   *absLonA = absLon.toArray(),
                   *absRA = absR.toArray(),
                   *wLatA = wLat.toArray(),
                   *wLonA = wLon.toArray(),
                   *wRA = wR.toArray();
            
            /* GLQ stuff */
            register int i, j, k; /* GLQ indexes */ 
            double tmpWLon, tmpWLat, /* Temps used to avoid using [] too much. this speeds up the program */
                   scaleFactor, /* The scale factor used because of the variable change made to tranform the the integration interval to [-1,1] */
                   res; /* Aux for the sum */
                   
            /* Model element temporary holders */
            double eW, eE, eS, eN, eTop, eBottom; /* The model element bounds. Keep them here to avoid calling get*() all the time */
                              
            /* Grid stuff */
            unsigned int clat, clon; /* grid position counters */
            double phi, lamb, r = R + Z, /* Computation point coordinates (grid nodes). r is mean earth radius plus elevation */
                   latStep, lonStep, /* Interval between the grid nodes */ 
                   Wrad = deg2rad*W, /*Erad = deg2rad*E,*/ Srad = deg2rad*S/*, Nrad = deg2rad*N*/; /* The grid bounds in radians */
                               
            /* aux for calculating KV */
            double rl, phil, lambl, rl_2, 
                   r_2,
                   cosPhil,
                   cosPhi,                   
                   sinPhil,
                   sinPhi,
                   cosLambLambl,       
                   cosPsi,          
                   cosPsiPhi,  
                   l,
                   l_3,
                   rl_l3,                        
                   KV;
                   
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
                eTop = R-mod[e].getTop();
                eBottom = R-mod[e].getBottom();
                              
                /* Scale the abscissas for this model element */                
                absLat.scale(eS, eN);
                absLon.scale(eW, eE);
                absR.scale(eBottom, eTop);
                
                /* Calculate the scale factor */
                scaleFactor = (si2eotvos*(eN-eS)*(eE-eW)*(eTop-eBottom)*mod[e].getDensity()*G) / 8;
                             
                /* Calculte the field caused by this element on the data grid */
                for(phi=Srad, clat=0; /*lat<=Nrad && */clat<nlat; phi+=latStep, clat++){
                    for(lamb=Wrad, clon=0; /*lon<=Erad && */clon<nlon; lamb+=lonStep, clon++){
                        
                        /* Do the GLQ */
                        res = 0;
                        for(i=0; i<latOrder; i++){
                            phil = absLatA[i];
                            tmpWLat = wLatA[i];
                            for(j=0; j<lonOrder; j++){
                                lambl = absLonA[j];
                                tmpWLon = wLonA[j];
                                for(k=0; k<rOrder; k++){
                                    /* Calculate KV */
                                    rl = absRA[k];
                                    rl_2 = rl*rl;
                                    r_2 = r*r;
                                    cosPhil = cos(phil);
                                    cosPhi = cos(phi);                
                                    sinPhil = sin(phil);
                                    sinPhi = sin(phi);
                                    cosLambLambl = cos(lamb-lambl);
                                    cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl;
                                    cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl;
                                    l = sqrt( (r_2) + (rl_2) - (2*r*rl*cosPsi) );
                                    l_3 = l*l*l;
                                    rl_l3 = (rl)/l_3;
                                                                       
                                    KV = rl_2*cosPhil*( (((r*rl_l3*cosPsiPhi)/r) - (rl_l3*cosPsiPhi*((3*r*((rl*cosPsi) - r)/(l*l)) + 1))) / (r) );   
                                    
                                    res += tmpWLat*tmpWLon*wRA[k]*KV;
                                }
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
double GxzCalculator3D::si2eotvos = 1000000000;

#endif

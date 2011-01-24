#ifndef _WEIGHTS_H_
#define _WEIGHTS_H_

/* ****************************************************************************************** */
/* ************************************ Weights ********************************************* */
/* ******* Calculate and store the weights for the Gauss-Legendre Quadrature (GLQ) ********** */
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


#ifndef _ABSCISSAS_H_
#include "Abscissas.h"
#endif
#ifndef _EXCEPTIONS_H_
#include "exceptions.h"
#endif
#ifndef _MATH_H_
#include<math.h>
#endif


class Weights {

    /* PRIVATE PARTS */
    private:
        
        /* FIELDS */
        /* ************************************************************** */
        
        /* The array of weights */
        double *w;
        /* Order of the GLQ */
        int N; 
          
        /* ************************************************************** */
    
    
        /* PRIVATE METHODS */
        /* ************************************************************** */
        
        void calcWeights(Abscissas& abs);
        
        /* ************************************************************** */
        
        
    /* PUBLIC PARTS */
    public:
        
        /* CONSTRUCTORS */
        /* ************************************************************** */
        
        Weights() {
            /* Create abscissas with the default order */
            Abscissas abs;
            /* Calculate the weights for those abs */
            calcWeights(abs);
        }
        
        Weights(Abscissas& abs) {                       
            /* Calculate the weights */
            calcWeights(abs);
        }
        
        ~Weights(){
            delete[] w;
        }
        
        /* ************************************************************** */
        
        /* METHODS */   
        /* ************************************************************** */
        
        /* Return the weight at position i */
        double at(int i){
            return w[i];
        }
        
        /* Return the address of the first abs. This is so i can use abs as an array, which is faster */
        double *toArray(){
            return w;
        }
        
        /* The number of weights there are */
        int size(){
            return N;
        }
                               
        /* ************************************************************** */
};


/* Calculate the weights */
/* wi = 2/(1-xi^2)(Pn'(xi))^2 */
void Weights::calcWeights(Abscissas& abs) {
                           
    register int i, n;
            
    double xi, /* xi is the ith abscissa (abs.elementAt(i)) */      
           pn, /* Legendre polynomial Pn(x) */
           pn_2, /* Pn-2 */        
           pn_1, /* Pn-1 */  
           pn_line; /* Derivative of Pn(x) */
            
    /* Get the order from the abscissas */
    N = abs.size();
            
    /* Allocate memory for the weights */
    w = new double[N];
                            
    /* Calculate the N weights */
    for(i=0; i<N; i++){
                                        
        /* Need to find Pn'(xi) */              
        /* To do this, use the recursive relation to find Pn and Pn-1: */
        /*   Pn(x)=(2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n   */ 
        /* Then use:   Pn'(x)=n(xPn(x)-Pn_1(x))/(x*x-1) */
    
        xi = abs.uat(i);
                
        /* Find Pn and Pn-1 stating from P0 and P1 */
        pn_1 = 1;   /* This is Po(x) */
        pn = xi;    /* and this P1(x) */
        for(n=2; n<=N; n++){
            pn_2 = pn_1;
            pn_1 = pn;
            pn=( ((2*n-1)*xi*pn_1) - ((n-1)*pn_2) )/n;
        }
                
        /* Now find Pn'(xi) */
        pn_line = N*(xi*pn-pn_1)/(xi*xi-1);
                
        /* Calculate the weight Wi */
        /* wi = 2/(1-xi^2)(Pn'(xi)^2) */
        w[i] = 2/( (1-xi*xi)*(pn_line*pn_line) );
    }
}

#endif

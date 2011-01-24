#ifndef _ABSCISSAS_H_
#define _ABSCISSAS_H_

/* ****************************************************************************************** */
/* ************************************ Abscissas ******************************************* */
/* ****************************************************************************************** */
/* ********* Calculate and store abscissas for the Gauss-Legendre Quadrature (GLQ) ********** */
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



#ifndef _MATH_H_
#include<math.h>
#endif
#ifndef _STDIO_H_
#include<stdio.h>
#endif
#ifndef _EXCEPTIONS_H_
#include "exceptions.h"
#endif

#ifndef L_MODULO
#define L_MODULO(a) (((a)>=0)?(a):((-1)*(a)))
#endif

class Abscissas {
    
    /* PRIVATE PARTS */
    private:
        
        /* FIELDS */
        /* ************************************************************** */
        
        /* Order of the GLQ */
        int N; 
        /* Array with the abscissas */
        double *abs;
        /* To keep the unscaled abscissas */
        double *unscaledAbs;        
        /* Marker to say if the abscissas were scaled or not */
        bool absScaled;
        /* Maximum number of iterations used in Newton's Method */
        static int maxIterations; 
        /* The maximum error accepted in Newton's Method (1E-15) */
        static double error;
        /* The default order of the GLQ */
        static int defaultOrder;
        /* Pi */
        static double pi;
                
        /* ************************************************************** */
        
        /* PRIVATE METHODS */
        /* ************************************************************** */
        
        /* The calcAbscissas needs to know the order */
        void calcAbscissas(int newOrder){
            
            register int i;
            
            /* At first the abscissas are in the range [-1,1] */
            absScaled = false;
            
            N = newOrder;
            
            /* Allocate memory for the abscissas */
            unscaledAbs = new double[N];
            abs = new double[N];
            
            for(i=0; i<N; i++){
                unscaledAbs[i] = findRoot(cos(pi*((i+1)-0.25)/(N+0.5)), i);            
                abs[i] = unscaledAbs[i];
            }
        }
              
        /* Fing the root of Pn(x) using */
        /* Newton's Method for Multiple Roots presented in                   */
        /* Barrera-Figueroa, V., Sosa-Pedroza, J. and LÃ³pez-Bonilla, J.,    */
        /* "Multiple root finder algorithm for Legendre and Chebyshev        */
        /* polynomials via Newton's method", 2006, Annales mathematicae      */  
        /* et Informaticae, 33, pp 3-13                                      */
        double findRoot(double initialGuess, int currentRoot){
                    
            double x1, /* Xn+1 used in Newton's Method */        
                   x0, /* Xn used in Newton's Method */
                   pn, /* Legendre polynomial Pn(x) */
                   pn_2, /* Pn-2 */        
                   pn_1, /* Pn-1 */  
                   pn_line, /* Derivative of Pn(x) */
                   sum; /* to represent the sum of previous roots (see Barrera-Figueroa et al.) */
            int iterations = 0; /* The number of iterations used to calculate the root */
            register int n; /* Counter */
            
            x1 = initialGuess;
            do{            
                x0=x1;
                
                /* Calculate Pn(x0) */
                /* ************************************************************** */
                /* Starting from P0(x) and P1(x), */
                /* find the others using the recursive relation: */
                /*     Pn(x)=(2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n   */  
                pn_1=1;   /* This is Po(x) */
                pn=x0;    /* and this P1(x) */
                for(n=2; n<=N; n++){
                    pn_2=pn_1;
                    pn_1=pn;
                    pn=( ((2*n-1)*x0*pn_1) - ((n-1)*pn_2) )/n;
                }
                /* ************************************************************** */
                
                /* Now calculate Pn'(x0) using another recursive relation: */
                /*     Pn'(x)=n(xPn(x)-Pn_1(x))/(x*x-1)                    */
                /* ************************************************************** */
                pn_line=N*(x0*pn-pn_1)/(x0*x0-1);
                /* ************************************************************** */
                
                /* Sum the roots found so far */
                sum=0;
                for(n=0; n<(currentRoot); n++){
                    sum+=1/(x0-abs[n]);
                }
                
                /* Update the guess for the root */
                x1=x0-(double)pn/(pn_line-pn*sum);
                
            }while(L_MODULO(x1-x0)>error && ++iterations<=maxIterations);
                       
            /* Tell the user if stagnation occurred */
            if(iterations > maxIterations){
                fprintf(stderr, "\n\nWARNING! Stagnation occurred when looking for root %d of Legendre polynomial P%d.", currentRoot+1, N);
                fprintf(stderr, "Maximum error of 1E-15 cannot be guaranteed.\n");
            }
           
            return x1;
        }  
                    
        /* ************************************************************** */
        

    /* PUBLIC PARTS */
    public:
        
        /* CONSTRUCTORS */
        /* ************************************************************** */
        
        Abscissas(){           
            /* Calculate the abscissas */
            calcAbscissas(defaultOrder);
        }
        
        Abscissas(int newOrder){                 
            /* Calculate the abscissas */
            calcAbscissas(newOrder);
        }
        
        ~Abscissas(){
            delete[] unscaledAbs;
            delete[] abs;
        }
        
        /* ************************************************************** */
    
    
        /* METHODS */   
        /* ************************************************************** */
       
        /* Return the abscissa at position i */
        double at(int i){
            return abs[i];
        }
        
        /* Return the address of the first abs. This is so i can use abs as an array, which is faster */
        double *toArray(){
            return abs;
        }
        
        /* Return the unscaled abscissa at position i */
        double uat(int i){
            return unscaledAbs[i];
        }
        
        /* The number of abscissas there are */
        int size(){
            return N;
        }
        
        /* Tell wether or not the abs are scaled */
        bool scaled(){
            return absScaled;
        }
                       
        /* Scale the abscissas to the interval [a,b] */
        /* using x=(b-a)u/2+(b+a)/2 */
        void scale(double a, double b) /*throw (StrangeAbsScaleException)*/ {
            
            register int i;
                                   
            absScaled = true;
            
            /* This is done to optimize the code */
            double tmpplus = (b+a) / 2, tmpminus = (b-a) / 2;
            
            for(i=0; i<N; i++){
                abs[i] = tmpminus*unscaledAbs[i] + tmpplus;
            }
            
            /* Now that I think about it, a>=b doesn't matter because the abs are simetric */
            
            /* If a >= b the integral might have it's sign altered! Warn the user! */
          /*  if(a >= b){
                StrangeAbsScaleException e(a,b);
                throw e; 
            }*/
        }
                
        /* ************************************************************** */
};

/* Maximum number of iterations used in Newton's Method */
int Abscissas::maxIterations = 10000; 
/* The maximum error accepted in Newton's Method (1E-18) */
double Abscissas::error = 0.000000000000001;
/* The default order of the GLQ */
int Abscissas::defaultOrder = 2;
/* Pi */
double Abscissas::pi = 3.1415926535897932384626433832795;


#endif

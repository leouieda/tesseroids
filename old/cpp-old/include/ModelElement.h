#ifndef _MODELELEMENT_H_
#define _MODELELEMENT_H_

/* ****************************************************************************************** */
/* ********************************** ModelElement ****************************************** */
/* ****************************************************************************************** */
/* ************************** Store info on a model element ********************************* */
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



#ifndef _STDIO_H_
#include<stdio.h>
#endif
#ifndef _EXCEPTIONS_H_
#include "exceptions.h"
#endif

class ModelElement {


    /* PRIVATE PARTS */
    private:
        
        /* FIELDS */
        /* ************************************************************** */
       
        double W, /* West bound of the element */
               E, /* East bound of the element */
               S, /* South bound of the element */
               N, /* North bound of the element */
               top, /* Top bound of the element */
               bottom, /* Bottom bound of the element */
               density; /* The density of the element */
        /* Indicate is the params were set */
        bool boundsSet, densSet;
        
        /* ************************************************************** */
        
        
    /* PUBLIC PARTS */
    public:
            
        /* CONSTRUCTORS */
        /* ************************************************************** */
        
        ModelElement(){
            boundsSet = false;
            densSet = false;
        }
        
        ModelElement(double newW, double newE, double newS, double newN, double newTop, double newBottom, double newDensity)
                                                                                throw (StrangeModelElementBoundsException){
            W = newW;
            E = newE;
            S = newS;
            N = newN;
            top = newTop;
            bottom = newBottom;
            density = newDensity;
            boundsSet = true;
            densSet = true;
            if((W >= E) || (top >= bottom) || (S >= N)){
                StrangeModelElementBoundsException e;
                throw e;
            }               
        }
        
        /* ************************************************************** */
    
    
        /* METHODS */   
        /* ************************************************************** */
       
        /* Set the boundary parameters of the element */
        void setBounds(double newW, double newE, double newS, double newN, double newTop, double newBottom) 
                                                                            throw (StrangeModelElementBoundsException){
            W = newW;
            E = newE;
            S = newS;
            N = newN;
            top = newTop;
            bottom = newBottom;           
            boundsSet = true;
            if((W >= E) || (top >= bottom) || (S >= N)){
                StrangeModelElementBoundsException e;
                throw e;
            }       
        }
        
        /* Set the density parameter */
        void setDensity(double newDensity){
            densSet = true;
            density = newDensity;
        }
        
        /* Return the params */
        double getW() throw (ElementParamNotSetException) {
            if(!boundsSet){
                ElementParamNotSetException e("west boundary");
                throw e;
            }
            return W;        
        }
        double getE() throw (ElementParamNotSetException) {
            if(!boundsSet){
                ElementParamNotSetException e("east boundary");
                throw e;
            }
            return E;        
        }
        double getS() throw (ElementParamNotSetException) {
            if(!boundsSet){
                ElementParamNotSetException e("south boundary");
                throw e;
            }
            return S;        
        }
        double getN() throw (ElementParamNotSetException) {
            if(!boundsSet){
                ElementParamNotSetException e("north boundary");
                throw e;
            }
            return N;        
        }
        double getTop() throw (ElementParamNotSetException) {
            if(!boundsSet){
                ElementParamNotSetException e("top boundary");
                throw e;
            }
            return top;        
        }
        double getBottom() throw (ElementParamNotSetException) {
            if(!boundsSet){
                ElementParamNotSetException e("bottom boundary");
                throw e;
            }
            return bottom;        
        }
        double getDensity() throw (ElementParamNotSetException) {
            if(!densSet){
                ElementParamNotSetException e("density");
                throw e;
            }
            return density;        
        }
        
        /* ************************************************************** */
};

#endif

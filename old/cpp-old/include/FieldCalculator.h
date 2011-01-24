#ifndef _FIELDCALCULATOR_H_
#define _FIELDCALCULATOR_H_
    
/* ****************************************************************************************** */
/* ******************************* FieldCalculator ****************************************** */
/* ****************************************************************************************** */
/* ********************* Keep information about the data grid ******************************* */
/* ********** Mother class to the one that calculate the actual gravity fields ************** */
/* ****************************************************************************************** */
/* *********************************** WARNING!!! ******************************************* */
/* ***** This is an abstract class! It's children should implement the 'calculate' method! ** */
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

#ifndef _EXCEPTIONS_H_
#include "exceptions.h"
#endif

#ifndef _DATA_H_
#include "Data.h"
#endif

#ifndef _MODELELEMENT_H_
#include "ModelElement.h"
#endif


class FieldCalculator {

    /* PROTECTED PARTS */
    protected:
        /* FIELDS */
        /* ************************************************************** */
        
        /* The grid bounds */
        double W, E, S, N;
        /* The hight */
        double Z;    
        /* The number of points in the lat and lon direction */
        unsigned int nlon, nlat;
        /* The total number of points */
        unsigned int n;      
        /* Flags to check if all the params were set */
        bool boundsSet, hightSet, dimSet;
        /* The gravitational constant (m^3*kg^-1*s^-1) */
        static double G;
        /* The mean earth radius (m) */
        static double R;
        /* Pi */
        static double pi;
        /* Conversion factor for degree to radian */
        static double deg2rad;
        
        /* ************************************************************** */
    
    /* PUBLIC PARTS */
    public:
        /* CONSTRUCTORS */
        /* ************************************************************** */
        
        FieldCalculator(){
            boundsSet = false;
            hightSet = false;
            dimSet = false;
        }
        
        FieldCalculator(double newW, double newE, double newS, double newN, double newZ, int newNlon, int newNlat){
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
        
        /* Set the parameters of the grid */
        void setBounds(double newW, double newE, double newS, double newN){
            W = newW;
            E = newE;
            S = newS;
            N = newN;
            boundsSet = true;
        }
        void setHight(double newZ){
            Z = newZ;
            hightSet = true;
        }
        void setDimension(int newNlon, int newNlat){
            nlon = newNlon;
            nlat = newNlat;    
            n = nlon * nlat;        
            dimSet = true;
        }
        
        /* Check if all params were set */
        bool paramsSet(){
            if(boundsSet == true && hightSet == true && dimSet == true){
                return true;
            }
            return false;
        }
        
        /* Get the size of the grid */
        int size() throw (ElementParamNotSetException){
            if(!dimSet){
                ElementParamNotSetException e("dimension");
                throw e;
            }
            return n;
        }
        int sizeLon() throw (ElementParamNotSetException){
            if(!dimSet){
                ElementParamNotSetException e("dimension");
                throw e;
            }
            return nlon;
        }
        int sizeLat() throw (ElementParamNotSetException){
            if(!dimSet){
                ElementParamNotSetException e("dimension");
                throw e;
            }
            return nlat;
        }
        
        /* The purely virtual prototype for the calculate method */
        /* Should receive a model vector by ref and return a data grid with the respective gravity field */
        virtual Data* calculate(vector<ModelElement>& mod_vector) throw (GridParamNotSetException) =0;
                                   
};

/* The gravitational constant (m^3*kg^-1*s^-1) */
double FieldCalculator::G = 0.00000000006673;
/* The mean earth radius (m) */
double FieldCalculator::R = 6378137;
/* Pi */
double FieldCalculator::pi = 3.1415926535897932384626433832795;
/* Conversion factor for degree to radian */
double FieldCalculator::deg2rad = pi/180;

#endif

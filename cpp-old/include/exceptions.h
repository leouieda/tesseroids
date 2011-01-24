#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

/* ****************************************************************************************** */
/* ************************************ Exceptions ****************************************** */
/* ****************************************************************************************** */
/* ********** Exceptions used in the tesseroid gravity modeling software package ************ */
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


#ifndef _GLIBCXX_STRING
#include<string>
#endif

#ifndef _STDLIB_H_
#include<stdlib.h>
#endif

/* MOTHER CLASS TO ALL EXCEPTIONS */
class MyException {
    
    protected:
        char m[1000];
        
    public:
        MyException(){
        }

        virtual char* msg(){
            return m;
        }
};

/* TERMINATE - Thrown when program needs to be terminated */
/* **************************************************************** */
class TerminateException: public MyException {
       
    public:
        TerminateException(){
            sprintf(m, "\nProgram terminated!\nDon't worry, he'll be back...\n");
        }       
        char* msg(){
            return m;
        }
};
/* **************************************************************** */


/* NO ELEMENTS IN MODEL - Thrown when there are no valid elements in the model input file */
/* **************************************************************** */
class NoElementsInModelException: public MyException {
       
    public:
        NoElementsInModelException(){
            sprintf(m, "\n\nERROR! There were no valid elements in the model input file!\n");
        }
        NoElementsInModelException(string filename){
            sprintf(m, "\n\nERROR! There were no valid elements in the model input file '%s'!\n", filename.c_str());
        }
        char* msg(){
            return m;
        }
};
/* **************************************************************** */


/* CANT OPEN FILE - Thrown when openning a file fails */
/* **************************************************************** */
class CantOpenFileException: public MyException {
       
    public:
        CantOpenFileException(){
            sprintf(m, "\n\nERROR! Couldn't open file!\n");
        }
        CantOpenFileException(string filename){
            sprintf(m, "\n\nERROR! Couldn't open file '%s'!\n",  filename.c_str());
        }
        char* msg(){
            return m;
        }
};
/* **************************************************************** */


/* PARAMS NOT OK - Thrown when any cmd param is wrong */
/* **************************************************************** */
class ParamsNotOKException: public MyException {
       
    public:
        ParamsNotOKException(){
            sprintf(m, "\n\nPlease check your command line!\nRun the program with no arguments for <a hopefully usefull> help.\n");
        }
        
        char* msg(){
            return m;
        }
};
/* **************************************************************** */


/* TOO MANY FILE NAMES - Thrown when more than 1 input file name is given */
/* **************************************************************** */
class TooManyFileNamesException: public MyException {
       
    public:
        TooManyFileNamesException(){
            sprintf(m, "\n\nERROR! More than one input file name was given!\nThe input file name is the ONLY arg that doesn't start with '-'!\n");
        }
        
        char* msg(){
            return m;
        }
};
/* **************************************************************** */


/* ELEMENT PARAM NOT SET - Thrown when trying to access a param that wasn't set */
/* **************************************************************** */
class ElementParamNotSetException: public MyException {
    
    public:
        ElementParamNotSetException(){
            sprintf(m, "\n\nERROR! Can't access this parameter because it hasn't been set yet!\n");
        }
        ElementParamNotSetException(string param){            
            sprintf(m, "\n\nERROR! Can't access parameter '%s' because it hasn't been set!\n", param.c_str());      
        }
        
        char* msg(){
            return m;
        }
};
/* **************************************************************** */


/* GRID PARAMS NOT SET - Thrown by FieldCalculator's children when trying to calculate */
/*                       a field without setting the grid parameters                   */
/* **************************************************************** */
class GridParamNotSetException: public MyException {
    
   public:
        GridParamNotSetException(){
            sprintf(m, "\n\nERROR! Can't calculate field without setting the grid parameters first!\n");
        }
        GridParamNotSetException(string param){            
            sprintf(m, "\n\nERROR! Can't calculate %s field without setting the grid parameters first!\n", param.c_str());
        }
        
        char* msg(){
            return m;
        }
};
/* **************************************************************** */


/* STRANGE MODEL ELEMENT BOUNDS - Thrown when bounds of the tesseroid might be wrong. */
/* **************************************************************** */
class StrangeModelElementBoundsException: public MyException {
   
    public:
        StrangeModelElementBoundsException(){
            sprintf(m, "\n\nWARNING! Bounds of element are invalid!\nThis element will not be considered!\n");
        }
                      
        char* msg(){
            return m;
        }

};
/* **************************************************************** */






/* UNSED EXCEPTIONS */

/* STRANGE ABSCISSAS SCALE - Thrown when trying to scale the abscissas with a >= b */
/* **************************************************************** */
class StrangeAbsScaleException: public MyException {
    
   public:
        StrangeAbsScaleException(){
            sprintf(m, "\n\nWARNING! Scaling abscissas <integrating from a to b> with a >= b!\nThis might lead to a zero or reversed sign integral!\n"); 
        }
        StrangeAbsScaleException(double a, double b){            
            sprintf(m, "\n\nWARNING! Scaling abscissas <integrating from a to b> with a >= b <a:%g|b:%g>!\nThis might lead to a zero or reversed sign integral!\n",a,b); 
        }
        
        char* msg(){
            return m;
        }

};
/* **************************************************************** */




#endif

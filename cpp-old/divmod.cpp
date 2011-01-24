/* ****************************************************************************************** */
/* ************************************* DivMod ********************************************* */
/* ****************************************************************************************** */
/* ************** Divide every tesseroid in a model file into smaller ones ****************** */
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

#include <stdio.h>
#include <stdlib.h>
#include<string>
#include<vector>
#include<iostream>
using namespace std;

#include "include/cmd.h"
#include "include/exceptions.h"
#include "include/ModelElement.h"

#ifndef L_MODULO
#define L_MODULO(a) (((a)>=0)?(a):((-1)*(a)))
#endif

/* FUNCTIONS */
vector<ModelElement> divideModel(vector<ModelElement>& mod, int lonO, int latO, int rO) throw (TerminateException){

    /* The new model */
    vector<ModelElement> newMod;    
    /* The dimensions of the new tesseroids */
    double lonSize, latSize, rSize;
    /* The bounds of the old tesseroid */
    double W, E, S, N, Top, Bottom, Dens;
    /* The bounds of the new tesseroid */
    double newW, newS, newTop;
    /* Convert the model to an array for faster manipulation */
    ModelElement *vmod = &mod[0];
    int modSize = mod.size();
    /* Model element counter */
    int e;  
    /* Temporary model element */
    ModelElement tmpE;
    /* Counters */
    register int i, j, k;

    for(e=0; e<modSize; e++){
        
        /* Get the bounds of the olde tesseroid */
        W = vmod[e].getW();
        E = vmod[e].getE();
        S = vmod[e].getS();
        N = vmod[e].getN();
        Top = vmod[e].getTop();
        Bottom = vmod[e].getBottom();
        /* Get the density of the old tesseroid */
        Dens = vmod[e].getDensity();
        
        /* Calculate the new dimensions of the tesseroids */
        lonSize = L_MODULO(E - W)/lonO;
        latSize = L_MODULO(N - S)/latO;
        rSize = L_MODULO(Bottom - Top)/rO;
        
        /* Cut the old tesseroid and put the new ones in the new mod */
        for(i=0,newW=W; i<lonO; i++,newW+=lonSize){
            for(j=0,newS=S; j<latO; j++,newS+=latSize){
                for(k=0,newTop=Top; k<rO; k++,newTop+=rSize){                            
                    try{
                        /* Place the new element in the new model */
                        tmpE.setBounds(newW,newW + lonSize,newS,newS + latSize,newTop,newTop + rSize);     
                        tmpE.setDensity(Dens);
                        newMod.push_back(tmpE);
                    } catch(StrangeModelElementBoundsException& ex){
                        fprintf(stderr, "\n\nOOPS! Got invalid bounds when dividing element %d!\n",e+1);
                        TerminateException te;
                        throw te;                        
                    }   
                }
            }
        }
    }

    return newMod;
}


/* MAIN */
/* ************************************************************************** */
int main(int argc, char* argv[]){
      
    if(argc == 1){        
        cerr << "\n  This is DivMod:";
        cerr << "\n      - Divide every tesseroid in a model file into smaller ones\n\n";
        cerr << "\n  Synopsis:\n";
        cerr << "\n   divmod.exe model_file -O<Lon/Lat/R>\n\n";
        cerr << "\n  Mandatory arguments:\n";
        cerr << "\n      model_file : The model input file name.";
        cerr << "\n";    
        cerr << "\n  Optional arguments:\n";
        cerr << "\n      -O : Division order. Into how many pieces each tesseroid will";
        cerr << "\n           be split in the longitude, latitude and radial directions.";
        cerr << "\n           Must be integers > 0 <DEFAULT = 2/2/2>.";        
        cerr << "\n";    
        cerr << "\n  Specifications:\n";
        cerr << "\n      - Output is printed to stdout as 'W E S N Top Bottom Density'.";              
        cerr << "\n      - Unit system for input is SI.";                
        cerr << "\n      - Latitude and longitude should be in degrees.";        
        cerr << "\n      - Input model is assumed to be composed of tesseroids.";     
        cerr << "\n      - Input model file format:";
        cerr << "\n          * Each line should contain info of ONLY ONE tesseroid.";
        cerr << "\n          * Tesseroid info format: W E S N Top Bottom Density.";
        cerr << "\n          * W < E, S < N, Bottom > Top.";
        cerr << "\n          * Top = R - Rtop.";
        cerr << "\n          * Bottom = R - Rbottom.";
        cerr << "\n          * R is the mean Earth radius 6378137 m.";
        cerr << "\n          * Each value above should be separated by space or tab.";        
        cerr << "\n          * Lines that begin with a # will be disregarded <comments>.";           
        cerr << "\n";        
        return 0;
    }
    try{
                
        /* FIELDS */
        /* ****************************************************************** */
        
        /* The tesseroids that make up the model and the new divided model */
        vector<ModelElement> mod, dividedMod;        
        /* Pointer to model input file */
        FILE* input;
        /* The input file name */
        string filename; 
        /* Division order */
        int latO = 2, lonO = 2, rO = 2;       
        /* Counters */
        int i, element;
        /* Booleans for cheching if all args are present */
        bool fileSet = false, orderSet = false, paramsOK = true;
                
        /* ****************************************************************** */
                       
                       
        /* Friendly welcome msg */       
        cerr << "\n  This is DivMod:";
        cerr << "\n      - Divide every tesseroid in a model file into smaller ones\n\n";
        
        
        /* COMMAND LINE PARSING */
        /* ****************************************************************** */        
        cerr << "\nParsing command line arguments...";
      
        /* Iterate over the arguments */
        for(i=1; i<argc; i++){
            
            switch(argv[i][0]){
                
                /* There are grid and algorithm params */
                case '-':                    
                    /* Now check which option it is */
                    switch(argv[i][1]){
                                                                    
                        /* DIVISION ORDER OPTION */
                        case 'O':
                            if(orderSet == false){      
                                orderSet = true;                                            
                                if(!parseO(&argv[i][2],&lonO,&latO,&rO)){
                                    paramsOK = false;
                                    fprintf(stderr, "\n\nDivision order provided: %s\nIt should be -O<Lon/Lat/R>!\nLon, Lat and R must be integer > 0!", argv[i]);
                                }
                            }
                            else{
                                paramsOK = false;
                                fprintf(stderr, "\n\nERROR! More than one division order parameter <-O> was given!");
                            }                             
                        break;
                                          
                        default:
                            paramsOK = false;
                            fprintf(stderr, "\n\nERROR! Invalid argument %s!", argv[i]);
                    }                    
                break;
                
                /* The arg with no '-' should be the input file name. */
                /* Check if only one was given */
                default:
                    if(fileSet == false){
                        fileSet = true;
                        filename = argv[i];
                    }
                    else { 
                        paramsOK = false;
                        fprintf(stderr, "\n\nERROR! More than one model file name was given!\nThe model file name is the ONLY argument that doesn't start with '-'!");
                    }
                break;                        
            }        
        }
        /* Check if all parameters were OK */
        if(!fileSet){
            paramsOK = false;
            fprintf(stderr, "\n\nERROR! Input model file name not given!");
        }       
        if(!paramsOK){
            ParamsNotOKException ex;
            throw ex;
        }
        
        fprintf(stderr, " Done!");
        
        /* Print what was read */
        fprintf(stderr, "\n\nModel file: %s", filename.c_str());        
        fprintf(stderr, "\nDivision order: lon:%d lat:%d r:%d", lonO,latO,rO);
      
        /* ****************************************************************** */
        
                       
        /* GETTING THE MODEL */
        /* ****************************************************************** */
        /* Status msg */
        cerr << "\n\nTrying to open model input file '" << filename << "'...";
        
        /* Open the input file. Warn the user if it goes wrong. */
        input = fopen(filename.c_str(), "r");
        if(input == NULL){
            CantOpenFileException ex(filename);
            throw ex;
        }
        
        /* Status msg */
        cerr << " Success!\nReading model from input file...";    
        
        /* Now, read the model from the input file */
        int elements_in_file;
        elements_in_file = readModel(input, mod);        
        /* Now that I don't need it anymore, close it */
        fclose(input);
        
        /* Check if there were any valid elements in the file */
        if(mod.size() <= 0){
            NoElementsInModelException ex;
            throw ex;
        }
        
         /* Notify how many elements were successfully read */
        fprintf(stderr," Done!\n%lu out of %d model elements read successfully!", mod.size(), elements_in_file);   
          
        /* Printf the model to stderr 
        try{         
                  
            vector<ModelElement>::iterator mit;
               
            cerr << "\nThis is what was read:";           
            cerr << "\n\n  n  |   W   |   E   |   S   |   N   |  Top  | Bottom | Density";
            cerr << "\n---------------------------------------------------------------";
            
            for(n=1, mit=mod.begin(); mit<mod.end(); mit++, n++){
                fprintf(stderr,"\n%5d|% g % g % g % g % g % g % g",n,mit->getW(),mit->getE(),mit->getS(),mit->getN(),mit->getTop(),mit->getBottom(),mit->getDensity()); 
            }
            
            cerr << endl;
            
        } catch(ElementParamNotSetException& e){
            cout << e.msg();
        }*/ 
                     
        /* ****************************************************************** */
        
        
        /* DIVIDING THE TESSEROIDS */
        /* ****************************************************************** */
        cerr << "\n\nDividing the tesseroids...";
        
        dividedMod = divideModel(mod,lonO,latO,rO);
        
        /* Notify how many elements the new model has */
        fprintf(stderr," Done!\nThere are %lu model elements in the new model!", dividedMod.size());   
        
        /* ****************************************************************** */
        
        
        /* PRINT DATA TO OUTPUT (for now, in gnuplot format) */
        /* ****************************************************************** */
        cerr << "\n\nPrinting data to output...";       
        
        /* Print a header for the file */
        fprintf(stdout, "# Tesseroid model file to be used in 'tess' applications. Modified with 'divmod.exe'.");
        fprintf(stdout, "\n# INFO:");
        fprintf(stdout, "\n#     Original model file: %s", filename.c_str());
        fprintf(stdout, "\n#     Number of elements in the original: %lu", mod.size());
        fprintf(stdout, "\n#     Original divided in: lon:%d lat:%d r:%d", lonO, latO, rO);
        fprintf(stdout, "\n#     Number of elements in this file: %lu", dividedMod.size());
        fprintf(stdout, "\n#\n#    W     E     S     N     Top   Bottom   Density");
        
        /* Printf the model elements */
        
        /* Convert the model to an array for faster manipulation */
        ModelElement *vmod = &dividedMod[0];
        int modSize = dividedMod.size();
        
        for(element=0; element<modSize; element++){
            fprintf(stdout, "\n%g %g %g %g %g %g %g", vmod[element].getW(),vmod[element].getE(),vmod[element].getS(),vmod[element].getN(),vmod[element].getTop(),vmod[element].getBottom(),vmod[element].getDensity());
        }
        
        /* Status msg */
        fprintf(stderr, " Done!");
        /* ****************************************************************** */
        
        
        /* CLEANING UP (save the envirounment!) */
        /* ****************************************************************** */
        /* Release the memory used */
              
        /* And that's that */
        cerr << "\n\nFinished!\n";
        /* ****************************************************************** */
        
      
    /* Catch the expetions thrown and print the error msgs */
    } catch(bad_alloc& e){
        cerr << "\n\nERROR! Bad memory allocation!\n";
        cerr << "\nProgram terminated!\nDon't worry, he'll be back...\n";
    } catch(NoElementsInModelException& e){
        cerr << e.msg();
        cerr << "\nProgram terminated!\nDon't worry, he'll be back...\n";    
    } catch(CantOpenFileException& e){
        cerr << e.msg();
        cerr << "\nProgram terminated!\nDon't worry, he'll be back...\n";    
    } catch(ParamsNotOKException& e){
        cerr << e.msg();
        cerr << "\nProgram terminated!\nDon't worry, he'll be back...\n";
    } catch(TerminateException& e){
        cerr << e.msg();   
    } catch(...){
        cerr << "\n\nAN UNEXPECTED ERROR OCCURRED!\n";
        cerr << "\nProgram terminated!\nDon't worry, he'll be back...\n";
    }
        
    return 0;
}
/* ************************************************************************** */

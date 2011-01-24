/* ****************************************************************************************** */
/* ************************************ TessGz ********************************************** */
/* ****************************************************************************************** */
/* ****** For calculating the vertical (gz) component of gravity of a tesseroid model ******* */
/* ****************************************************************************************** */
/* ************************** Developed by Leonardo Uieda *********************************** */
/* ****************************************************************************************** */
/* ************************* Geophysics undergraduate at the ******************************** */
/* ********** Institute of Astronomy, Geophysics and Atmospheric Sciences of the ************ */
/* **************************** University of São Paulo ************************************* */
/* ****************************************************************************************** */
/* ************************************** 2008 ********************************************** */
/* ****************************************************************************************** */
/* ************************** Compiled in Windows with g++ ********************************** */
/* ********************* To be tested in Linux Ubuntu and Solaris *************************** */
/* ****************************************************************************************** */

/* SPECIFICATIONS: */
/*   - Input model is assumed to be composed of tesseroids.                     */
/*   - Unit system for input is SI.                                             */
/*   - Unit for output is mGal.                                                 */
/*   - Elevation is positive upwards.                                           */
/*   - Latitude and longitude should be in degrees!                             */
/*   - For top and bottom of tesseroids, z is positive downwards.               */
/*   - Default GLQ order is 5.                                                  */
/*   - Default algotithm type is analytical integration in r and 2D GLQ.        */



#include<string>
#include<vector>
#include<iostream>
#include<time.h>
#include<stdio.h>
using namespace std;

#include "include/cmd.h"
#include "include/exceptions.h"
#include "include/Abscissas.h"
#include "include/Weights.h"
#include "include/ModelElement.h"
#include "include/Data.h"
#include "include/FieldCalculator.h"
#include "include/GzCalculator3D.h"
#include "include/GzCalculator2D.h"

/* Macro used to get linear position of a matrix index */
#ifndef POS
#define POS(i,j,ncols) (((i)*(ncols))+(j))
#endif

/* MAIN */
/* ************************************************************************** */
int main(int argc, char* argv[]){
      
    if(argc == 1){        
        cerr << "\n  This is TessGz:";
        cerr << "\n      - Calculate the vertical <gz> component of gravity of a tesseroid model\n\n";
        cerr << "\n  Synopsis:\n";
        cerr << "\n   tessgz.exe model_file -R<W/E/S/N> -B<nLon/nLat> -Z<hight> -O<Lon/Lat/R> -A<type>\n\n";
        cerr << "\n  Mandatory arguments:\n";
        cerr << "\n      model_file : The model input file name.";
        cerr << "\n      -R : The output data grid boundaries in degrees.";
        cerr << "\n      -B : The dimensions of the grid.";
        cerr << "\n           nLon and nLat must be integer > 0.";
        cerr << "\n      -Z : The hight of calculation.";
        cerr << "\n";    
        cerr << "\n  Optional arguments:\n";
        cerr << "\n      -O : The GLQ orders in the longitude, latitude and radial directions.";
        cerr << "\n           Must be an integer > 0 <DEFAULT=5/5/5>.";
        cerr << "\n      -A : The type of algorithm to use.";
        cerr << "\n           'type' may be either '2D' of '3D' GLQ <DEFAULT=2D>.";
        cerr << "\n";    
        cerr << "\n  Specifications:\n";
        cerr << "\n      - Output is printed to stdout as 'lon lat value'.";            
        cerr << "\n      - Unit system for input is SI.";        
        cerr << "\n      - Unit of output is mGal.";
        cerr << "\n      - Latitude and longitude should be in degrees.";
        cerr << "\n      - Hight of calculation is positive upwards.";
        cerr << "\n      - Default GLQ order is 5.";             
        cerr << "\n      - Algorithm types are:";
        cerr << "\n          * Analytical integration in r and 2D GLQ.";
        cerr << "\n          * 3D GLQ.";
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
        
        /* The tesseroids that make up the model */
        vector<ModelElement> mod;
        /* The data grid that will be output */       
        Data *grd;
        /* The field calculator */
        FieldCalculator *field;
        /* Witch kind of algorithm to use (DEFAULT = 2D GLQ)*/
        string algorithmType = "2D";
        /* Pointer to model input file */
        FILE* input;
        /* The input file name */
        string filename;
        /* The grid bounds and elevation */
        double W, E, S, N, Z;
        /* The grid dimensions */
        int sizeLon, sizeLat;
        /* Gauss-Legender Quadrature orders (DEFAULT = 5) */
        int lonO = 5, latO = 5, rO = 5;
        /* Time counters */
        clock_t start, stop;
        int hour, min;
        double sec;
        /* Counters */
        int l, c, i;
        /* Booleans for cheching if all args are present */
        bool regSet = false, dimSet = false, fileSet = false, hightSet = false, orderSet = false, algSet = false, paramsOK = true;
        
        
        /* ****************************************************************** */
                       
                       
        /* Friendly welcome msg */
        cerr << "\n  This is TessGz:";
        cerr << "\n      - Calculate the vertical <gz> component of gravity of a tesseroid model\n\n";
        
        
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
                    
                        /* GRID REGION OPTION */
                        case 'R':
                            if(regSet == false){      
                                regSet = true;                      
                                if(!parseR(&argv[i][2],&W,&E,&S,&N)){
                                    paramsOK = false;
                                    fprintf(stderr, "\n\nRegion provided: %s\nIt should be -R<W/E/S/N>!", argv[i]);
                                }
                            }
                            else{
                                paramsOK = false;
                                fprintf(stderr, "\n\nERROR! More than one region parameter <-R> was given!");
                            }                                                            
                        break;
                        
                        /* GRID DIMENSIONS OPTION */
                        case 'B':
                            if(dimSet == false){      
                                dimSet = true;                      
                                if(!parseB(&argv[i][2],&sizeLon,&sizeLat)){
                                    paramsOK = false;
                                    fprintf(stderr, "\n\nDimensions provided: %s\nIt should be -B<nLon/nLat>!\nnLon and nLat must be integer > 0!", argv[i]);
                                }
                            }
                            else{
                                paramsOK = false;
                                fprintf(stderr, "\n\nERROR! More than one grid dimensions parameter <-B> was given!");
                            }                                                            
                        break;
                        
                        /* GRID ELEVATION OPTION */
                        case 'Z':
                            if(hightSet == false){      
                                hightSet = true;                      
                                if(!parseZ(&argv[i][2],&Z)){
                                    paramsOK = false;
                                    fprintf(stderr, "\n\nHight of calculation provided: %s\nIt should be -Z<hight>!", argv[i]);
                                }
                            }
                            else{
                                paramsOK = false;
                                fprintf(stderr, "\n\nERROR! More than one hight of calculation parameter <-Z> was given!");
                            }                              
                        break;
                        
                        /* GLQ ORDER OPTION */
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
                                fprintf(stderr, "\n\nERROR! More than one GLQ order parameter <-O> was given!");
                            }                             
                        break;
                        
                        /* ALGORITHM TYPE ORDER OPTION */
                        case 'A':
                            if(algSet == false){
                                string tmpstr(&argv[i][2]);
                                if(tmpstr.compare("2D")!=0 && tmpstr.compare("3D")!=0){
                                    paramsOK = false;
                                    fprintf(stderr, "\n\nERROR! Invalid algorithm type!");
                                    fprintf(stderr, "\n\nType provided: %s\nIt should be -A<type>!\n'type' must be either '2D' or '3D'!", argv[i]);
                                }
                                else{
                                    algorithmType = tmpstr;
                                }                                
                            }
                            else{
                                paramsOK = false;
                                fprintf(stderr, "\n\nERROR! More than one algorithm type parameter <-A> was given!");
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
        if(!regSet){
            paramsOK = false;
            fprintf(stderr, "\n\nERROR! Region parameter -R not given!");
        }
        if(!hightSet){
            paramsOK = false;
            fprintf(stderr, "\n\nERROR! Hight of calculation parameter -Z not given!");
        }
        if(!dimSet){
            paramsOK = false;
            fprintf(stderr, "\n\nERROR! Grid dimensions parameter -B not given!");
        }
        if(!paramsOK){
            ParamsNotOKException ex;
            throw ex;
        }
        
        fprintf(stderr, " Done!");
        
        /* Print what was read */
        fprintf(stderr, "\n\nModel file: %s", filename.c_str());
        fprintf(stderr, "\nGrid region: W:%g E:%g S:%g N:%g", W, E, S, N);
        fprintf(stderr, "\nGrid dimensions: Lon:%d x Lat:%d", sizeLon, sizeLat);
        fprintf(stderr, "\nHight of calculation: %g m", Z);
        fprintf(stderr, "\nAlgorithm type: %s GLQ", algorithmType.c_str());
        fprintf(stderr, "\nGLQ orders: lon:%d lat:%d r:%d", lonO,latO,rO);
      
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
       
        /* Notify how many elements were successfully read */
        fprintf(stderr," Done!\n%lu out of %d model elements read successfully!", mod.size(), elements_in_file);
        
        /* ****************************************************************** */
        
        
        /* CALCULATING THE GRAVITY FIELD */
        /* ****************************************************************** */
    
        /* First, choose the right field to calculate */       
        if(algorithmType.compare("3D")==0){   
                     
            field = new GzCalculator3D(latO,lonO,rO);
            /* Status msg */
            fprintf(stderr, "\n\nCalculating field using 3D GLQ...");    
                   
        } else if(algorithmType.compare("2D")==0){    
                   
            field = new GzCalculator2D(latO,lonO);
            /* Status msg */
            fprintf(stderr, "\n\nCalculating field using 2D GLQ...");       
                 
        }
        
        /* Next, set the grid parameters */
        field->setBounds(W,E,S,N);
        field->setHight(Z);
        field->setDimension(sizeLon, sizeLat);
                    
        /* Only then, calculate the field and clock the time */
        start = clock();
        grd = field->calculate(mod);
        stop = clock();    
       
        /* Status msg */
        sec = (double)(stop - start)/CLOCKS_PER_SEC;
        hour = (int)(sec) / 3600;
        min = ((int)(sec)/60) - hour*60;
        sec = sec - min*60 - hour*3600;        
        fprintf(stderr, " Done! <time %d:%02d:%06.3lf> ", hour, min, sec);
    
        /* ****************************************************************** */
        
        
        /* PRINT DATA TO OUTPUT (for now, in gnuplot format) */
        /* ****************************************************************** */
        cerr << "\n\nPrinting data to output...";       
        for(l=0; l<sizeLat; l++){
            for(c=0; c<sizeLon; c++){
                fprintf(stdout, "%g %g %g\n", grd[POS(l,c,sizeLon)].getLon(), grd[POS(l,c,sizeLon)].getLat(), grd[POS(l,c,sizeLon)].getValue());
            }
            fprintf(stdout, "\n");
        }        
        /* Status msg */
        fprintf(stderr, " Done!");
        /* ****************************************************************** */
        
        
        /* CLEANING UP (save the envirounment!) */
        /* ****************************************************************** */
        /* Release the memory used */
        delete [] grd;
        delete field;
        
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
    } catch(GridParamNotSetException& e){
        cerr << e.msg();
        cerr << "\nProgram terminated!\nDon't worry, he'll be back...\n";    
    } catch(...){
        cerr << "\n\nAN UNEXPECTED ERROR OCCURRED!\n";
        cerr << "\nProgram terminated!\nDon't worry, he'll be back...\n";
    }
        
    return 0;
}
/* ************************************************************************** */

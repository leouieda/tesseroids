#ifndef _CMD_H_
#define _CMD_H_

/* ****************************************************************************************** */
/* **************************** LEO'S CMD PARSING TOOLS ************************************* */
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


#ifndef _MODELELEMENT_H_
#include "ModelElement.h"
#endif


/* Parse the HIGHT PARAMETER out of the command line */
/* Pass to it the hight param WITHOUT the -Z part! */
/* ************************************************************************** */
bool parseZ(char *arg, double *Z){
       
    int i;
    char tmp[100];
    bool dot = false, sign = false, OK = true;
    
    /* Get the hight */   
    for(i=0; arg[i]!='\0'; i++){
        /* Check for invalid characters */
        if((arg[i]<'0' || arg[i]>'9') && arg[i]!='+' && arg[i]!='-' && arg[i]!='.'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in hight parameter -Z!", arg[i]);
            OK = false;
        }
        if(arg[i]=='+' || arg[i]=='-'){
            if(sign){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in hight parameter -Z!", arg[i]);
                OK = false;
            }
            else{
                sign = true;
            }
        }     
        if(arg[i]=='.'){
            if(dot){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in hight parameter -Z!", arg[i]);
                OK = false;
            }
            else{
                dot = true;
            }
        }             
        tmp[i] = arg[i];        
    }
    tmp[i] = '\0';
    if(i == 0){
        fprintf(stderr, "\n\nERROR! Hight of calculation parameter -Z has no value!");
        return false;
    }
    if(OK){
        *Z = (double)atof(tmp);
    }
   
    return OK;
}
/* ************************************************************************** */


/* Parse the GRID DIMENSIONS PARAMETER (nLon/nLat) out of the command line */
/* Pass to it the dimension param WITHOUT the -B part! */
/* ************************************************************************** */
bool parseB(char *arg, int *nLon, int *nLat){
    
    bool LonOK = true, LatOK = true;
    int i, j;
    char tmp[100];
       
    /* Get the nLon */    
    for(i=0, j=0; arg[i]!='/' && arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if(arg[i]<'0' || arg[i]>'9'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in grid dimension parameter -B!", arg[i]);
            LonOK = false;
        }       
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(arg[i] == '\0' || j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in grid dimension parameter -B!");
        return false;
    }
    if(LonOK){
        *nLon = atoi(tmp);
    }
                   
    /* Get the nLat */  
    for(i++, j=0; arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if(arg[i]<'0' || arg[i]>'9'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in grid dimension parameter -B!", arg[i]);
            LatOK = false;
        }       
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in grid dimension parameter -B!");
        return false;
    }
    if(LatOK){
        *nLat = atoi(tmp);
    }
   
   
    /* Check if everything is OK */    
    if(LatOK && LonOK){
        if(*nLon > 0 && *nLat > 0){
            return true;
        }
        fprintf(stderr, "\n\nERROR! Invalid grid dimension parameter -B!");
    }
    return false;
}
/* ************************************************************************** */


/* Parse the REGION PARAMETER (W/E/S/N) out of the command line */
/* Pass to it the region param WITHOUT the -R part! */
/* ************************************************************************** */
bool parseR(char *arg, double *W, double *E, double *S, double *N){
    
    bool Wok = true, Eok = true, Sok = true, Nok = true;
    int i, j;
    char tmp[100];
    bool dot, sign;
    
    /* Get the W */
    dot = false;
    sign = false;  
    for(i=0, j=0; arg[i]!='/' && arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if((arg[i]<'0' || arg[i]>'9') && arg[i]!='+' && arg[i]!='-' && arg[i]!='.'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
            Wok = false;
        }
        if(arg[i]=='+' || arg[i]=='-'){
            if(sign){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                sign = true;
            }
        }     
        if(arg[i]=='.'){
            if(dot){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                dot = true;
            }
        }             
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(arg[i] == '\0' || j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in region parameter -R!");
        return false;
    }
    if(Wok){
        *W = (double)atof(tmp);
    }
       
    /* Get the E */
    dot = false;
    sign = false;  
    for(i++, j=0; arg[i]!='/' && arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if((arg[i]<'0' || arg[i]>'9') && arg[i]!='+' && arg[i]!='-' && arg[i]!='.'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
            Eok = false;
        }
        if(arg[i]=='+' || arg[i]=='-'){
            if(sign){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                sign = true;
            }
        }     
        if(arg[i]=='.'){
            if(dot){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                dot = true;
            }
        }             
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(arg[i] == '\0' || j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in region parameter -R!");
        return false;
    }
    if(Eok){
        *E = (double)atof(tmp);
    }
    
    /* Get the S */
    dot = false;
    sign = false;  
    for(i++, j=0; arg[i]!='/' && arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if((arg[i]<'0' || arg[i]>'9') && arg[i]!='+' && arg[i]!='-' && arg[i]!='.'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
            Sok = false;
        }
        if(arg[i]=='+' || arg[i]=='-'){
            if(sign){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                sign = true;
            }
        }     
        if(arg[i]=='.'){
            if(dot){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                dot = true;
            }
        }             
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(arg[i] == '\0' || j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in region parameter -R!");
        return false;
    }
    if(Sok){
        *S = (double)atof(tmp);
    }
         
    /* Get the N */
    dot = false;
    sign = false;  
    for(i++, j=0; arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if((arg[i]<'0' || arg[i]>'9') && arg[i]!='+' && arg[i]!='-' && arg[i]!='.'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
            Nok = false;
        }
        if(arg[i]=='+' || arg[i]=='-'){
            if(sign){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                sign = true;
            }
        }     
        if(arg[i]=='.'){
            if(dot){
                fprintf(stderr, "\n\nERROR! Invalid character '%c' in region parameter -R!", arg[i]);
                Wok = false;
            }
            else{
                dot = true;
            }
        }             
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in region parameter -R!");
        return false;
    }
    if(Nok){
        *N = (double)atof(tmp);
    }
   
   
    /* Check if everything is OK */    
    if(Wok && Eok && Sok && Nok){
        return true;
    }
    else{
        return false;
    }
}
/* ************************************************************************** */



/* Parse the ORDER PARAMETER (LonO/LatO/rO) out of the command line */
/* Pass to it the ORDER param WITHOUT the -O part! */
/* ************************************************************************** */
bool parseO(char *arg, int *lonO, int *latO, int *rO){
    
    bool LonOK = true, LatOK = true, ROK = true;
    int i, j;
    char tmp[100];
       
    /* Get the LonO */    
    for(i=0, j=0; arg[i]!='/' && arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if(arg[i]<'0' || arg[i]>'9'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in order parameter -O!", arg[i]);
            LonOK = false;
        }       
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(arg[i] == '\0' || j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in order parameter -O!");
        return false;
    }
    if(LonOK){
        *lonO = atoi(tmp);
    }
    
    /* Get the latO */    
    for(i++, j=0; arg[i]!='/' && arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if(arg[i]<'0' || arg[i]>'9'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in order parameter -O!", arg[i]);
            LatOK = false;
        }       
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(arg[i] == '\0' || j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in order parameter -O!");
        return false;
    }
    if(LatOK){
        *latO = atoi(tmp);
    }
                   
    /* Get the rO */  
    for(i++, j=0; arg[i]!='\0'; i++, j++){
        /* Check for invalid characters */
        if(arg[i]<'0' || arg[i]>'9'){
            fprintf(stderr, "\n\nERROR! Invalid character '%c' in order parameter -O!", arg[i]);
            ROK = false;
        }       
        tmp[j] = arg[i];        
    }
    tmp[j] = '\0';
    if(j == 0){
        fprintf(stderr, "\n\nERROR! Too few arguments in order parameter -O!");
        return false;
    }
    if(ROK){
        *rO = atoi(tmp);
    }
   
   
    /* Check if everything is OK */    
    if(LatOK && LonOK && ROK){
        if(*lonO > 0 && *latO > 0 && *rO > 0){
            return true;
        }
        fprintf(stderr, "\n\nERROR! Invalid order parameter -O!");
    }
    return false;
}
/* ************************************************************************** */



/* Read the model from input file. Returns the number of elements originaly in the file */
/* ************************************************************************** */
int readModel(FILE* file, vector<ModelElement>& mod){
    
    /* String to hold the current line of the file */
    char line[5000], *aux;
    /* Line counter */
    int l, num_el;
    /* Temporary holders for the parameters */
    double tmpW, tmpE, tmpS, tmpN, tmpTop, tmpBot, tmpDens;
    /* Temporary model element */
    ModelElement e;
          
    /* Until the end of file */
    for(l=1, num_el=1; !feof(file); l++, num_el++){
        
        /* Read a line and catch the return in aux */        
        aux = fgets(line, 5000, file);
        
        /* Check if the line is a comment. If it is, ignore it */
        if(line[0]=='#'){
            num_el--;
            continue;
        }
                    
        /* Try to read the 7 parameters from the line */
        if((aux==NULL) || (sscanf(line, "%lf %lf %lf %lf %lf %lf %lf",&tmpW,&tmpE,&tmpS,&tmpN,&tmpTop,&tmpBot,&tmpDens)) != 7){
            fprintf(stderr, "\n\nWARNING! Invalid and/or missing data in line %d!\nThis element will not be considered!\n", l);
            continue;            
        } 
        
        /* If all 7 parameters were there, put them in the model */
        try{
            e.setBounds(tmpW,tmpE,tmpS,tmpN,tmpTop,tmpBot);     
            e.setDensity(tmpDens);
            mod.push_back(e);
        } catch(StrangeModelElementBoundsException& e){
            fprintf(stderr, "\n\nWARNING! Bounds of element %lu are invalid!\nThis element will not be considered!\n",mod.size()+1);
        }
    }
    
    return num_el-1;   
}
/* ************************************************************************** */




#endif

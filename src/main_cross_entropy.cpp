/**
    main_cross_entropy.cpp
    Purpose: Computes cross entropy using 2 input files
            1) Receives Alpha, k vector and lambda;
            2) Reads inputs;
            3) Normalizes inputs
            4) Computes cross entropy.
           
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/



#include "parseArgs.h"
#include "crss_ent.h"

int main (int argc, char **argv){

    Args argument = parseArgs(argc,argv);
    printArgs(argument);
    crss_ent crss_ent(argument);
}
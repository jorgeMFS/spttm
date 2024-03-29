/**
    compute_nrc.cpp
    Purpose: Computes Normalize Relative Compression (NRC) using 2 input strings
            1) Receives Alpha and k vector;
            2) Reads inputs;
            3) Computes NRC;
           
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include "parseArgs.h"
#include "nrc.h"

int main (int argc, char **argv){

    Args argument = parseArgs(argc,argv);
    printArgs(argument);
    nrc nrc(argument);
}
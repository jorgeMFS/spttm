/**
    spark.cpp
    Purpose: Run Basic Search Program
            1) Receives input to search;
            2) 2 Flags: A) Search options (Sequential, Monte carlo or Neural Network);B) threshold.
            3) Searchs space using KL divergence (various k).
            4) When threshold is obtained stops.
            5) Outputs best results.
    
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include "search.h"
#include "util.h"

int main (int argc, char **argv){

    Args argument = parseArgs(argc,argv);
    //printArgs(argument);
    double weight = 0.5;
    Search search(argument,weight);
}



/**
    main_cross_entropy.cpp
    Purpose: Computes  Kullback-Leibler divergency using 1 target file and 1 input file
            1) Receives Alpha, k vector and lambda;
            2) Reads inputs;
            3) Normalizes inputs
            4) Computes Kullback-Leibler divergency for:
                a)Conditional probability 
                b) Probability of sequence of prefix + alfabet letter.
           
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/



#include "parseArgs.h"
#include "kullbackLeiblerDivergency.h"

int main (int argc, char **argv){

    Args argument = parseArgs(argc,argv);
    printArgs(argument);
    KullbackLeiblerDivergency kullbackLeiblerDivergency(argument, false);
    kullbackLeiblerDivergency.run_w_input_file();
}
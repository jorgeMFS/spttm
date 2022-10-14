/**
    markovTable.h
    Purpose: Performes Monte-Carlo Search in a given domain of TM(sigma,state).
    
    Steps:
    A) Given a target string Y, it creates a pool of random TM.
    B) Each TM runs n times. After running n iterations, the loss is determined for each iteration, by using KullbackLeiblerDivergence.
    C) When a match is found, or the threshold met, the machine stops, and outputs: (the statematrix, the divergence value, the tape output and run time.) 

    Optional, have time limit for running the machines. (i.e. parameter of runs or parameters for time)

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/


#include "parseArgs.h"


struct MonteCarloSearch{
    MonteCarloSearch(Args args);

    private:
        void init();
        //Search
        //MultiCore

};


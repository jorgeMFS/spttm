/**
    markovTable.h
    Purpose: Performes Sequential Search in a given domain of TM(sigma,state).
    
    Steps:
    A) Given a target string Y, it performs a sequential traversal search for the full TMs in the domain TM(sigma,state).
    B) Each TM runs n times. After running n iterations, the loss is determined for each iteration, by using KullbackLeiblerDivergence.
    C) When a match is found, or the threshold met, the machine stops, and outputs: (the statematrix, the divergence value, the tape output and run time.) 
    
    Optional, have time limit for running the machines. (i.e. parameter of runs or parameters for time)


    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/


#include "interactiveMarkovModel.h"
#include "parseArgs.h"
#include "loss.h"

struct Search{
    
    Args args;
    Loss loss;
    TmId traversal_len;
    Search(Args args, double weight);
    
    private:
        void init();
        void SequentialSearch();
        void MonteCarloSearch();
        double test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models);
        //Search
        //MultiCore

};


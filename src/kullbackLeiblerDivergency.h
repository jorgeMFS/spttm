/**
    kullbackLeiblerDivergency.h
    Purpose: Computes Kullback–Leibler divergence:
    Mode A) Conditional Probabilities of Markov Models
    Mode B) Probability of k+1 elements.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once

#include "parseArgs.h"
#include "stringProcess.h"
#include "markovTable.h"


struct KullbackLeiblerDivergency{

    
    KullbackLeiblerDivergency(Args args);

    public:
        void run_w_input_file();
        std::vector<double> compute_divergency_pconditional(std::vector<MarkovTable> &mk_input_vector) const;
        std::vector<double> compute_divergency_p_k_elem(std::vector<MarkovTable> &mk_input_vector) const;

    private:
        StringProcess target;
        Args args;
        std::vector<MarkovTable> mk_target_vector;
        
        void init();
        double compute_divergency_pconditional(MarkovTable &mk_input, MarkovTable &mk_target) const;
        double compute_divergency_p_k_elem(MarkovTable &mk_input, MarkovTable &mk_target) const;
};


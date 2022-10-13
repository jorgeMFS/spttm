/**
    compute_nrc.cpp
    Purpose: Computes Kullback–Leibler divergence:
    Mode A) Conditional Probabilities of Markov Models
    Mode B) Probability of k+1 elements.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include "parseArgs.h"
#include "stringProcess.h"
#include "markovTable.h"


struct KullbackLeiblerDivergency{

    StringProcess target;
    KullbackLeiblerDivergency(Args args);
    
    private:
        void init(Args args);
        double compute_crss_ent(MarkovTable &mk_input, MarkovTable &mk_target, double &lambda) const;
        double compute_divergency_pconditional(MarkovTable &mk_input, MarkovTable &mk_target, double &lambda) const;
        double compute_divergency_p_k_elem(MarkovTable &mk_input, MarkovTable &mk_target) const;
        std::vector<double> compute_divergency_pconditional(std::vector<MarkovTable> &mk_input_vector, std::vector<MarkovTable> &mk_target_vector, double &lambda) const;
        std::vector<double> compute_divergency_p_k_elem(std::vector<MarkovTable> &mk_input_vector, std::vector<MarkovTable> &mk_target_vector, double &lambda) const;
};

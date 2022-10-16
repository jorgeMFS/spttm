/**
    loss.h
    Purpose: Computes loss.
    Weight parameter define computations.
    Weight = 1, only computes Kullback–Leibler divergence conditional probabilities.
    Weight = 0, only computes Kullback–Leibler divergence probability of k+1 elements.
    Weight =]0,1[ does both, with weighted average.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/


#pragma once

#include "kullbackLeiblerDivergency.h"

struct Loss{
    
    Loss(Args &args, double &weight);
    
    public:
        double compute_loss(std::vector<MarkovTable> &mkv_table_vector);
    private:
        double w;
        KullbackLeiblerDivergency kl;
        double average_divergence_vector(std::vector<double> &divergence_vector);
};


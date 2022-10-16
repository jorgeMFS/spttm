/**
    genetic_algorithm.h
    Purpose: Genetic algorithm that looks for tapes.

    @author Jorge Miguel Ferreira da Silva
    @version 1.0
*/
// This is it, is the do or die, its 

#pragma once

#include <set>
#include <vector>
#include "stringProcess.h"
// #include "cost.h"
#include "parseArgs.h"
// #include "complex_tm.h"



struct compare_pair_NRC{
    bool operator()(const std::pair<std::string, double> & n1, const std::pair<std::string, double> & n2);
};

struct compare_pair_nc{
    bool operator()(const std::pair<std::string, double> & n1, const std::pair<std::string, double> & n2);
};


struct GeneticTM{
    GeneticTM(Args args);
    GeneticTM(StateMatrix rule_matrix,
                    unsigned int tape_it,
                    std::vector<unsigned int> vector_k,
                    const double alpha,
                    StringProcess strProcess,
                    const unsigned int sd,
                    const unsigned int tm_it,
                    const unsigned int jobs,
                    const double min_f,
                    const double alpha_nc,
                    const unsigned int min_tape_sz,
                    const unsigned int max_tape_sz,
                    const double  nc_threshold,
                    const unsigned int init_sons
                    );
    
    //varibles
    //Cost cost_calculator;
    unsigned int tape_iterations;
    std::vector<unsigned int>vector_k;
    unsigned int seed;
    const unsigned int search_it;
    const unsigned int jobs;
    const double f_min;
    const unsigned int alph_sz;
    const unsigned int nb_states;
    const unsigned int init_sons;
    const unsigned int min_tape_sz;
    const unsigned int max_tape_sz;
    //ComplexTM tm_imp;

    std::set<std::pair<std::string,double>,compare_pair_NRC> open_set;
    std::set<std::string> closed_set;

    public:
        void genetic_increase();

    private:
        void create_initial_pool(const unsigned int & n);
        void create_sons(StateMatrix rm1, StateMatrix sm_m, const unsigned int & number_of_sons);
        void extinction_event(unsigned int index_range, double extinction_threshold);
        
        Metrics run_metrics(StateMatrix const & st, unsigned int const & it);
        std::pair<std::string, double> open_set_top_pop();
        std::vector<StateMatrix> get_top_n_del_rest(unsigned int & top_n);
        void open_set_remove(unsigned int index);
        std::pair<std::string, double> open_set_get(unsigned int index);
        void read_from_file(std::string filename);
        void print_open_set() const;
        std::pair<std::string, double> open_set_top();
        void erase_bottom_n(const unsigned int n);

};


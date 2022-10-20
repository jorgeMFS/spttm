/**
    sptm.h
    Purpose: Get inputs parameters, output metrics.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once
#include <vector>
#include "parseArgs.h"
#include "stringProcess.h"

// receives files
// updates TM
// outputs NRC
// waits to receive more files
// recalculates NCR

struct sptm{
    
    StringProcess str;
    unsigned int index;
    unsigned int num_it;
    unsigned int input_amplitude;
    std::vector<double> per_elem_input;
    sptm(Args args);
    void update(); 
    double get_diff_amplitudes(unsigned int & tape_length);
    std::vector<double> percentage_elements(std::vector<unsigned int> const & written_tape, int const & alphabet_size);
    std::vector<double> perc_diff_elems(std::vector<double> const & v1, std::vector<double> const & v2);
    std::pair<double, double>  hit_score_and_dist(std::vector<unsigned int> const & v1, std::vector<unsigned int> const & v2);
};
    double hit_score(std::vector<unsigned int> const & v1, std::vector<unsigned int> const & v2);
    std::vector<unsigned int >parse_context(std::string context_string);
    std::tuple<StateMatrix, unsigned long long, std::vector<unsigned int>, double, unsigned int> read_file() ;
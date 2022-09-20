/**
    parseArgs.h
    Purpose: Parse input arguments

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once
#include <iostream>
#include <utility>

#include "turingMachine.h"
#include "tmId.h"

struct Args{
    
    unsigned int sd = 0;
    size_t states = 0;
    size_t alphabet_size = 0;
    unsigned int tape_iterations = 0;
    std::vector <unsigned int> k;
    double alpha = 1;
    TmId n = 0;
    unsigned int c = 0;
    std::string input_file ="";
    unsigned int n_files=0;
    unsigned int num_out_lines=0;
    double threshold;
};

void help();
Args parseArgs (int argc, char **argv);
void printArgs(Args);

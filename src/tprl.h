/**
    loss.h
    Purpose: Produces TMs without loops. Has 2 modes.
    Mode Y) Produces file with TM rule matrix, rules index used in each iteration, tapes produces after n iterations,
    and position tm is in each iterations.
    Mode N) Produces TM tape after n iteration, cerr the tm matrix.
    
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/


#pragma once

#include "turingMachine.h"
#include "markovTable.h"
#include "parseArgs.h"
#include "stringProcess.h"
struct tprl{
    
    bool print_all_bool;
    unsigned int index;
    unsigned int num_it;
    unsigned int input_amplitude;
    unsigned int counter;
    std::vector<double> per_elem_input;
    StateMatrix st;
    std::minstd_rand rng;

    tprl(Args args, bool print_all_bool);
    void step(Args args);
    void step_2(Args args);
};
    std::tuple<StateMatrix, unsigned long long, std::vector<unsigned int>, double, unsigned int>read_file();
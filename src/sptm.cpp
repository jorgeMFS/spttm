/**
    sptm.cpp
    Purpose: Get inputs parameters, output metrics.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/



#include "sptm.h"
#include "turingMachine.h"
#include "markovTable.h"
#include "interactiveMarkovModel.h"
#include "read_input.h"
#include <sstream>


sptm::sptm(Args args):str(args.input_file,args.alphabet_size){
    
    StateMatrix st(args.states, args.alphabet_size);
    
    std::minstd_rand rng{args.sd};
    input_amplitude = str.get_input_vector().size();
    st.set_random(rng);
    // st.print();

    TuringMachine tm(st);
    MarkovTable mrkvTable(args.k[0], args.alphabet_size, args.alpha);
    InteractiveMarkovModel model(args.k[0], args.alphabet_size, args.alpha);
    TapeMoves tpMove;
    for (auto i = 0u; i < args.tape_iterations; ++i){
        tpMove = tm.act(); // grave esti antaŭe
        model.update_table(tpMove, tm.turingTape);
    }
    auto mdl = model.get_markov_table();
    auto tape_length = tm.get_tape_size();
    auto diff = get_diff_amplitudes(tape_length);    auto nrc_vector = this->str.readinput(mdl);
    index = std::min_element(nrc_vector.begin(), nrc_vector.end()) - nrc_vector.begin();
    std::cout << nrc_vector[index] << "\t" << diff << std::endl;
    sptm::update();
}

void sptm::update(){

    auto input=read_file();
    auto st = std::get<0>(input);
    auto number_iterations = std::get<1>(input);
    auto k = std::get<2>(input);
    auto alpha = std::get<3>(input); 
    // auto sd = std::get<4>(input);

    InteractiveMarkovModel model(k, st.get_alphabet(), alpha);
    TuringMachine tm(st);
    TapeMoves tpMove;
    for (auto i = 0u; i < number_iterations; ++i){
        tpMove = tm.act(); // grave esti antaŭe
        model.update_table(tpMove, tm.turingTape);
    }
    auto tape_length = tm.get_tape_size();
    auto diff = get_diff_amplitudes(tape_length);
    auto mdl = model.get_markov_table();
    auto nrc_value = this->str.readinput(mdl,index);
    std::cout << nrc_value << "\t" << diff << std::endl;
}; 

std::tuple<StateMatrix, unsigned long long, unsigned int, double, unsigned int> sptm::read_file() {
    std::string line;
    std::tuple<StateMatrix, unsigned long long, unsigned int, double, unsigned int> candidate;
    std::getline (std::cin, line);
    std::istringstream iss(line); //put line into stringstream
    std::string string_state_matrix;
    unsigned int n_alphabet;
    unsigned int n_states;
    unsigned long long number_iterations;
    unsigned int context;
    double alpha;
    unsigned int sd;
    if(iss >> string_state_matrix >> n_alphabet >> n_states >> number_iterations >> context >> alpha >> sd){
        StateMatrix rule_matrix(string_state_matrix, n_states, n_alphabet); 
        std::tuple<StateMatrix, unsigned long long, unsigned int, double, unsigned int>input(rule_matrix, number_iterations,context, alpha, sd);
        candidate=input;        
    }
    else {
        throw std::runtime_error("Invalid data file");
        exit(-1);
    }
    
    // std::cout << "string_state_matrix :" << std::get<0>(candidate).get_state_matrix_string() << std::endl;
    // std::cout << "n_states :" << std::get<0>(candidate).get_number_states() << std::endl;
    // std::cout << "n_alphabet :" << std::get<0>(candidate).get_alphabet() << std::endl;
    // std::cout << "number_iterations :" << std::get<1>(candidate)  << std::endl;
    // std::cout << "context :" << std::get<2>(candidate) << std::endl;
    // std::cout << "alpha :" << std::get<3>(candidate) << std::endl;
    // std::cout << "sd :" << std::get<4>(candidate) << std::endl;
    return candidate;
};

double sptm::get_diff_amplitudes(unsigned int & tape_length){
    auto diff = std::abs(static_cast<double>(tape_length)-static_cast<double>(input_amplitude));
    auto max = std::max(static_cast<double>(tape_length),static_cast<double>(input_amplitude));
    return diff/max;
}
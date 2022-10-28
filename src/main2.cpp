/**
    main2.cpp
    Purpose: Reads from file of flag -f and reproduces TM;
    Compiles into: ./run
    Shape of input file:
    rule Matrix alphabet n_states n_tape_iterations 
    x-x-x-(..)	2	50	1000
    Instructions to run:
    ./run -f "FILE"
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/


#include <fstream>
#include <sstream>

#include "turingMachine.h"
#include "parseArgs.h"
#include "interactiveMarkovModel.h"
#include "stringProcess.h"


std::tuple<std::string, unsigned int, unsigned int, unsigned int>read_state_matrix_to_print(std::string fileName){

    std::vector<std::string> vecOfStrs;
    // Open the File
    std::ifstream in(fileName.c_str());
    // Check if object is valid
    
    if(!in){
        std::cerr << "Cannot Open the File : " << fileName <<std::endl;
        exit(-1);
    }
    std::string str;
    std::string string_state_matrix;
    unsigned int n_alphabet;
    unsigned int n_states;
    unsigned long long number_iterations;
    
    // Read the next line from File untill it reaches the end.

    while (std::getline(in, str)){
        
        std::istringstream iss(str); 
        if(iss >> string_state_matrix >> n_alphabet >> n_states >> number_iterations){          
            std::tuple<std::string, unsigned int, unsigned int, unsigned int>reproduce(string_state_matrix, n_states, n_alphabet, number_iterations);
            return reproduce;
        }
        else {
            throw std::runtime_error("Invalid data file");
            exit(-1);
        }
       break;
    }
    in.close();
    return std::tuple<std::string, unsigned int, unsigned int, unsigned int>(string_state_matrix, n_states, n_alphabet, number_iterations);
}

int main (int argc, char **argv){
    Args argument = parseArgs(argc,argv);

    
    auto input=read_state_matrix_to_print(argument.input_file);
    auto st_string = std::get<0>(input);
    auto n_states = std::get<1>(input);
    auto n_alphabet = std::get<2>(input);
    auto number_iterations = std::get<3>(input);

    StringProcess strProcess(argument.target_file, n_alphabet);

    StateMatrix st(st_string, n_states, n_alphabet );
    TuringMachine tm(st);
    TapeMoves tpMove;

    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(argument.k, n_alphabet, argument.alpha);
    all_models.reset();

    for (auto i = 0u; i < number_iterations; ++i){
        tpMove = tm.act(); // grave esti antaŭe
        all_models.update_tables(tpMove, tm.turingTape);
    }
    tm.print_written_tape(true);

    std::vector<MarkovTable> mkv_vector = all_models.get_markov_tables();

    std::vector<double> bits_per_k;
    double cy_x;
    for (auto &markovtable : mkv_vector){

        auto cy_x_values = strProcess.readinput_cd(markovtable);
        auto cy = strProcess.normalizer;
        cy_x = *std::min_element(cy_x_values.begin(), cy_x_values.end());
        
        bits_per_k.push_back(std::min(cy_x, cy));

        //std::cerr <<std::endl;
        //for (auto &bits: bits_per_k){
        //    std::cerr <<"bits " << bits <<" cy "<< cy << " cy_x " << cy_x << std::endl;
       // }
    }
    auto min_bits_per_k_cy_x = std::ceil(*std::min_element(bits_per_k.begin(), bits_per_k.end()));
    std::cerr <<min_bits_per_k_cy_x << std::endl;
    auto alphabet_bits = std::ceil(log2(n_alphabet));
    auto states_bits = std::ceil(log2(n_states));
    auto moves_bits = 2.0;

    std::cerr <<n_alphabet << " " << n_states << std::endl;

    auto c_algo = (alphabet_bits+states_bits+moves_bits)*(n_alphabet*n_states) + std::ceil(log2(number_iterations));
    std::cerr <<c_algo << std::endl;
    auto c = c_algo + min_bits_per_k_cy_x;

    std::cout << c  << "," << strProcess.normalizer << std::endl;

    return 0;
}
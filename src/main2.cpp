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

    StateMatrix st(st_string, n_states, n_alphabet );
    TuringMachine tm(st);
    TapeMoves tpMove;
    for (auto i = 0u; i < number_iterations; ++i){
        tpMove = tm.act(); // grave esti antaŭe
    }
    tm.print_written_tape(true);
    return 1;
}
/**
    parseArgs.cpp
    Purpose: Parse input arguments

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <getopt.h>
#include <cstring>
#include <iostream>
#include <iomanip>      // std::setw
#include <cassert>
#include "util.h"
#include "parseArgs.h"
#define assertm(exp, msg) assert(((void)msg, exp))

Args parseArgs (int argc, char **argv){
    Args argument {};    
    opterr = 0;

    char *end;
    TmId correctInput;
    while (1){
        static struct option long_options[] =
        {

            {"help",      no_argument,      0, 'h'},
            {"number_states", optional_argument, 0 , 's'},
            {"alphabet_size", optional_argument, 0, 'a'},
            {"iterations", optional_argument, 0, 'i'},
            {"context", required_argument, 0, 'k'},
            {"seed",optional_argument,0,'e'},
            {"alpha", required_argument, 0 , 'A'},
            {"number_output_lines", optional_argument, 0, 'o'},
            {"threshold", optional_argument, 0, 'T'},
            {"input_file", optional_argument, 0 ,'f'},
            {"target_file", optional_argument, 0 ,'t'},
            {"number_files", optional_argument, 0 ,'N'},
            {"strategy", optional_argument, 0, 'S'},
            {NULL, 0, NULL, 0}
        };
        int option_index = 0;

        int c = getopt_long (argc, argv, "e:s:a:A:i:o:T:k:f:t:N:S:h",
                        long_options, &option_index);

        if (c == -1)
        break;
        
    switch (c)
        {
        case 0:
            if(long_options[option_index].flag != 0)
            break;
            printf ("option %s", long_options[option_index].name);

            if(optarg)
            printf(" with arg %s", optarg);
            printf("\n");
            break;

        case 'h':
            help();
            exit (0);
        
        case 'e':
        {
            correctInput = strtol(optarg, &end, 10);
            if (*end != '\0') {
                std::cerr << "Invalid input for -e/--seed." << std::endl;
                exit(0);
            }
            else if (correctInput<=0){
                std::cerr << "-e/--seed value was set to " << correctInput <<", must be an int larger than 0.\n";
                exit(0);
            }
            else argument.sd = correctInput;
            break;
        }

        case 's':
            {
            correctInput = strtol(optarg, &end, 10);
            if (*end != '\0') {
                std::cerr << "Invalid input for -s/--number_states.\n";
                exit(0);
            }
            else if (correctInput<=0){
                std::cerr << "-s/--number_states value was set to " << correctInput <<", must be an int larger than 0.\n";
                exit(0);
            }
            else argument.states = correctInput;
            break;
            }

        case 'a':
            {
            correctInput = strtol(optarg, &end, 10);
            if (*end != '\0') {
                std::cerr << "Invalid input for -a/--alphabet_size.\n";
                exit(0);
            }
            else if (correctInput<=0){
                std::cerr << "-a/--alphabet_size value was set to " << correctInput <<", must be an int larger than 0.\n";
                exit(0);
            }
            else argument.alphabet_size = correctInput;
            break;
        }
        case 'A':
            {
            correctInput = strtol(optarg, &end, 10);
            if (*end != '\0') {
                std::cerr << "Invalid input for -A/--alpha.\n";
                exit(0);
            }
            else if (correctInput<=0 || correctInput>5000){
                std::cerr << "-A/--alpha value was set to " << correctInput <<", must be an int larger than 0 and smaller than 5001.\n";
                exit(0);
            }
            else {
                argument.alpha = static_cast<double>(1.0)/correctInput;
            }
            break;
        }
        case 'i':
            {
            correctInput = strtol(optarg, &end, 10);
            if (*end != '\0') {
                std::cerr << "Invalid input for -i/--iterations.\n";
                exit(0);
            }
            else if (correctInput<=0){
                std::cerr << "-i/--iterations value was set to " << correctInput <<", must be an int larger than 0.\n";
                exit(0);
            }
            else argument.tape_iterations = correctInput;
            break;
        }
        case 'N':
            {
            correctInput = strtol(optarg, &end, 10);
            if (*end != '\0') {
                std::cerr << "Invalid input for -N/--n_files.\n";
                exit(0);
            }
            else if (correctInput<=0){
                std::cerr << "-N/--n_files value was set to " << correctInput <<", must be an int larger than 0.\n";
                exit(0);
            }
            else argument.n_files = correctInput;
            break;
        }
        case 'o':
            {
            correctInput = strtol(optarg, &end, 10);
            if (*end != '\0') {
                std::cerr << "Invalid input for -o/--number_output_lines.\n";
                exit(0);
            }
            else if (correctInput<=0){
                std::cerr << "-o/--number_output_lines value was set to " << correctInput <<", must be an int larger than 0.\n";
                exit(0);
            }
            else argument.num_out_lines = correctInput;
            break;
        }
        
        case 'T':
            {
            double correctInput = strtof(optarg, &end);
            if (*end != '\0') {
            std::cerr << "invalid input for -T/--threshold.\n";
            exit(0);
            }
            else if (correctInput<0 || correctInput>1){
            printf ("-t/--threshold value was set to %f, must be a double that belongs to the interval [0,1].\n",correctInput); 
            exit(0);
            }
            else {
                argument.threshold = correctInput;
                }
            break;
        }

        case 'k':
        {   
            std::pair<unsigned int,unsigned int> k_limits;
            std::vector<unsigned int> k_values;
            if (std::strchr(optarg, ':')){
                auto token = std::strtok (optarg," :");
                unsigned int counter=0;
                
                while (token != NULL)
                {
                    if (counter>1){
                        std::cerr << "Invalid input for -k/--context." << std::endl <<
                        "If you want to provide a range for k " <<
                         "please provide -k/--context with 2 elements separeted by \":\".\n" <<
                         "Example: -k 2:4"<< std::endl;
                        exit(0);
                    }
                    else{
                        correctInput = strtol(token, &end, 10);
                        if (*end != '\0') {
                            std::cerr << "Invalid input for -k/--context.\n";
                            exit(0);
                        }
                        else if (correctInput<=0){
                            std::cerr << "-k/--context value was set to " << correctInput <<", must be an int larger than 0.\n";
                            exit(0);
                        }
                        else {
                            if (counter==0) k_limits.first=correctInput;
                            else if (counter==1) k_limits.second=correctInput;
                        };
                    }
                    token = strtok (NULL, ":");
                    ++counter;
                }
            }            
            else{
                correctInput = strtol(optarg, &end, 10);
                if (*end != '\0') {
                    std::cerr << "Invalid input for -k/--context.\n";
                    exit(0);
                    }
                else if (correctInput<=0){
                    std::cerr << "-k/--context value was set to " << correctInput <<", must be an int larger than 0.\n";
                    exit(0);
                }
                else {
                    k_limits.first = correctInput;
                    k_limits.second = correctInput; 
                }
            }

            if (k_limits.first<=k_limits.second){
                for (auto it=k_limits.first; it!=k_limits.second+1;++it){
                    k_values.push_back(it);
                }
            }
            argument.k = k_values;
            break;
        }
        
        case 'f':
        {
            // std::string input_file = optarg;
            argument.input_file = optarg;
            break;
        }
        case 't':
        {
            argument.target_file = optarg;
            break;
        }
        case 'S':
        {
            if (std::strcmp(optarg, "S") == 0) {
                argument.search_strategy = "Sequential";
            } else if (std::strcmp(optarg, "M") == 0) {
                argument.search_strategy = "monte_carlo";
            } else if (std::strcmp(optarg, "N") == 0) {
                argument.search_strategy = "neural_networks";
            } else {
                std::cerr << "-S/--strategy must be either `sequential` or `monte_carlo`." << std::endl;
                exit(0);
            }
            break;
        }

        case '?':
            break;

        default:
            abort ();
        }
    }

    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
        printf ("%s ", argv[optind++]);
        putchar ('\n');
    }

    for (auto kval: argument.k){
        if(ipow<TmId>(argument.alphabet_size, kval) >= ipow<TmId>(2,28)){
            fprintf (stderr, "k/context (%u) and Alphabet size/a (%zu) is too large..\n", kval, argument.alphabet_size);
            fprintf (stderr, " please consider a size of a^k (%zu^%u) smaller than 2^28..\n", argument.alphabet_size,kval);
            exit(0);
        }
    }
    if(argument.tape_iterations>0 & argument.num_out_lines>0){
        assertm(((argument.tape_iterations % argument.num_out_lines)==0), "num_out_lines must be mod of tape_iterations");
    }
    return argument;
}



void printArgs(Args arguments){
    std::cout<< bold_on << "Seed = " << arguments.sd << bold_off << std::endl;
    std::cout<< "States = " << arguments.states << std::endl;
    std::cout<< "Alphabet Size = " << arguments.alphabet_size<< std::endl;
    std::cout<< "Tape Iterations = " << arguments.tape_iterations << std::endl;
    std::cout<< "Input file : " << arguments.input_file << std::endl;
    std::cout<< "Target file : " << arguments.target_file << std::endl;
    std::cout<< "Number of files : " << arguments.n_files << std::endl;
    std::cout<< "Threshold : " << arguments.threshold << std::endl;
    if (arguments.k.size()>1){
        std::cout<<"k = " << arguments.k.front()<<  ":" << arguments.k.back() << std::endl;
    }
    else if (arguments.k.size()==1){
        std::cout<<"k = " << arguments.k.front() << std::endl;
    }
    std::cout<< "Alpha = " << arguments.alpha << std::endl;
    std::cout<< "Number of output lines = " << arguments.num_out_lines << std::endl;
    std::cout<< "Search Strategy = " << arguments.search_strategy << std::endl;
}

void help(){

    std::cerr << std::endl << "                                                                             " << std::endl;
    std::cerr <<green_on;
    std::cerr << "                                                     ███████ ██████  ████████ ███    ███  "<< std::endl;
    std::cerr << "                                                     ██      ██   ██    ██    ████  ████ "<< std::endl;
    std::cerr << "                                                     ███████ ██████     ██    ██ ████ ██ "<< std::endl;
    std::cerr << "                                                          ██ ██         ██    ██  ██  ██ "<< std::endl;
    std::cerr << "                                                     ███████ ██         ██    ██      ██ " << std::endl <<bold_off<<std::endl<<std::endl;
    std::cerr << bold_on << green_on <<"                                 TURING MACHINE FINDER" <<bold_off<<std::endl <<std::endl<< std::endl;
    std::cerr << bold_on <<"                                                                     >"<< underlined_on <<"ARGUMENTS"<<bold_off <<"<                                          "<<bold_off << std::endl<<std::endl;
    std::cerr << bold_on << underlined_on << "Flags:" << bold_off << std::endl<<std::endl;
    std::cerr << bold_on << underlined_on << "Mandatory  Arguments:" << bold_off << std::endl << std::endl;
    std::cerr << bold_on << "-s" << ", " <<  "--number_states" << bold_off << "\t" << "Number of States the Turing Machine has."<< std::endl << std::endl;
    std::cerr << bold_on << "-a" << ", " <<  "--alphabet_size" << bold_off << "\t" << "Alphabet size the Turing Machine considers." << std::endl<<std::endl;
    std::cerr << bold_on << "-i" << ", " <<  "--iterations" << bold_off << "\t " << "Number of iterations the Turing Machine makes in the tape."<< std::endl<<std::endl;
    std::cerr << bold_on << "-k" << ", " <<  "--context" << bold_off << "\t" << "k indexes to consider as a context to fill the Markov Table." <<std::endl<<std::endl;
    std::cerr << bold_on << "-A" << ", " <<  "--alpha" << bold_off<< "\t" << "(integer [1;5000]) denominator to build alpha, which is a parameter estimator." <<std::endl;
    std::cerr <<"\t\t" << "Alpha is given by 1/<d>. Higher values are usually used with higher context <k>, "<<std::endl;
    std::cerr <<"\t\t" << "and related to confiant bets.When <d> is one, the probabilities assume a Laplacian distribution."<<std::endl<<std::endl;

    std::cerr << bold_on <<"-------------------------------------------------------------------------------------------"<<bold_off << std::endl<<std::endl;
    std::cerr << bold_on <<"                                        "<< underlined_on <<"EXAMPLES"<<bold_off <<"                                          "<<bold_off <<std::endl<< std::endl;

    std::cerr << "Get TM NRC"<< std::endl;
    std::cerr << "-----------------------------------------------------------------------------" << std::endl;
    std::cerr << ""<< std::endl;
    std::cerr << "echo 1000010000110001 | ./sptm   -s 2 -a 2 -i 10000 -k 2:10 -e 51654651" << std::endl;
}
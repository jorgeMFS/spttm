/**
    main_search.cpp
    Purpose: Computes nrc using 2 input strings
            1) Receives Alpha and k vector;
            2) reads inputs;
            3) Computes NRC;
           
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/


#include "nrc.h"
#include "parseArgs.h"
#include "read_input.h"
#include "interactiveMarkovModel.h"

nrc::nrc(Args args):str(args.target_file, args.alphabet_size){
    init(args);
}

void nrc::init(Args args){
    ReadInput readinput(args.input_file);
    ReadInput readtarget(args.target_file);

    auto input_vector = readinput.get_input_vector_as_unsigned_int();
    auto target_vector = readtarget.get_input_vector_as_unsigned_int();
    
    InteractiveMarkovModel model(args.k[0], args.alphabet_size, args.alpha);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);

    model.fill_with_vector(input_vector);
    model.get_markov_table().print();
    model.get_markov_table().normalize(0.0001);
    all_models.fill_with_vector(input_vector);

    auto mdl = model.get_markov_table();
    auto mdls = all_models.get_markov_tables();
    auto nrc_vector = this->str.readinput(mdl);
    auto index = std::min_element(nrc_vector.begin(), nrc_vector.end()) - nrc_vector.begin();
    auto nrc_values = this->str.readinput_multimarkovmodel(mdls,index);
    
    std::cout << "NRC value -> k:" << args.k.front() <<":"<< args.k.back()<<std::endl;
    for(auto &x:nrc_values){
                std::cout << x << "\t";
    }
    
}



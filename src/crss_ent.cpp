#include "crss_ent.h"
#include "parseArgs.h"
#include "read_input.h"
#include "interactiveMarkovModel.h"

crss_ent::crss_ent(Args args):str(args.target_file, args.alphabet_size){
    init(args);
}

void crss_ent::init(Args args){
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

    std::cout << "NRC value -> k:" << args.k.front() <<":"<< args.k.back()<<std::endl;
    for(auto &x:nrc_values){
                std::cout << x << "\t";
    }
    
}



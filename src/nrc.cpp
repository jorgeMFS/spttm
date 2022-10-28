#include "nrc.h"
#include "parseArgs.h"
#include "read_input.h"
#include "interactiveMarkovModel.h"

nrc::nrc(Args args):str(args.target_file, args.alphabet_size){
    init(args);
}

void nrc::init(Args args){
    ReadInput readinput(args.input_file); // string da tape
    //ReadInput readtarget(args.target_file);

    auto input_vector = readinput.get_input_vector_as_unsigned_int(); // tape em uint
    
    InteractiveMarkovModel model(args.k[0], args.alphabet_size, args.alpha);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);

    model.fill_with_vector(input_vector); // criar a tabela de markov dado a tape input_vector
    all_models.fill_with_vector(input_vector); // criar a tabela de markov dado a tape input_vector

    auto mdl = model.get_markov_table(); // saca a tabela de markov
    auto mdls = all_models.get_markov_tables(); // saca as tabelas de markov

    // TODO not very correct
    auto nrc_vector = this->str.readinput(mdl);
    auto index = std::min_element(nrc_vector.begin(), nrc_vector.end()) - nrc_vector.begin();
    auto nrc_values = this->str.readinput_multimarkovmodel(mdls,index);
    
    std::cout << "NRC value -> k:" << args.k.front() <<":"<< args.k.back()<<std::endl;
    for(auto &x:nrc_values){
                std::cout << x << "\t";
    }
    
}



#include "kullbackLeiblerDivergency.h"
#include "parseArgs.h"
#include "read_input.h"
#include "interactiveMarkovModel.h"

KullbackLeiblerDivergency::KullbackLeiblerDivergency(Args args):target(args.target_file, args.alphabet_size),args(args){
    init();
}

void KullbackLeiblerDivergency::init(){
    ReadInput readtarget(args.target_file);
    readtarget.printData();
    auto target_vector = readtarget.get_input_vector_as_unsigned_int();
    bool print_bool = false;
    mk_target_vector = target.get_models(args, target_vector , print_bool);

}

void KullbackLeiblerDivergency::run_w_input_file(){
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
    ReadInput readinput(args.input_file); //not in init;

    auto input_vector = readinput.get_input_vector_as_unsigned_int(); //not in init;
    all_models.fill_with_vector(input_vector); //not in init;
    auto mk_all_models_input_vectors = all_models.get_markov_tables(); //not in init;

    auto pconditional_divergence_vectors = compute_divergency_pconditional(mk_all_models_input_vectors); //not in init;
    auto p_k_elem_divergence_vectors = compute_divergency_p_k_elem(mk_all_models_input_vectors); //not in init;

    std::cout << "Relative Entropy [conditional Probability] -> k (" << args.k.front() <<":"<< args.k.back()<< ")" <<std::endl;

    for (auto i=0u;i<pconditional_divergence_vectors.size();++i){
        std::cout << args.k[i] << ":" << pconditional_divergence_vectors[i] << std::endl;
    }
    std::cout << "-------" << std::endl;

    std::cout << "Relative Entropy [Probability k+1 context] -> k (" << args.k.front() <<":"<< args.k.back()<< ")" <<std::endl;

    for (auto i=0u;i<p_k_elem_divergence_vectors.size();++i){
        std::cout << args.k[i] << ":" << p_k_elem_divergence_vectors[i] << std::endl;
    }
    std::cout << "-------" << std::endl;


}


std::vector<double> KullbackLeiblerDivergency::compute_divergency_pconditional(std::vector<MarkovTable> &mk_input_vector) const{
    std::vector<double> relative_entropy;

    for(auto i_in = 0u; i_in<mk_input_vector.size(); ++i_in ){
        
            auto mk_input=mk_input_vector[i_in];
            auto mk_target=mk_target_vector[i_in];
            relative_entropy.push_back(compute_divergency_pconditional(mk_input,mk_target));
        }
    return relative_entropy;
}



std::vector<double> KullbackLeiblerDivergency::compute_divergency_p_k_elem(std::vector<MarkovTable> &mk_input_vector) const{
    std::vector<double> relative_entropy;

    for(auto i_in = 0u; i_in<mk_input_vector.size(); ++i_in ){
        
        auto mk_input=mk_input_vector[i_in];
        auto mk_target=mk_target_vector[i_in];
        relative_entropy.push_back(compute_divergency_p_k_elem(mk_input,mk_target));
    }
    return relative_entropy;
}
double KullbackLeiblerDivergency::compute_divergency_pconditional(MarkovTable &mk_input, MarkovTable &mk_target) const{
    unsigned int c_size= pow(mk_input.alphSz,mk_input.get_context());
    //std::cout << c_size << std::endl;
    unsigned int alphabet_size= mk_input.alphSz;

    mk_input.normalize(args.lambda);
    mk_target.normalize(args.lambda);

    double sum_probability= 0;
    for (auto i=0u; i<c_size;++i){
        for (auto j=0u; j<alphabet_size;++j){

                auto val= mk_target.get_value_from_normalized_fcm(i,j) * log2( mk_target.get_value_from_normalized_fcm(i,j)/mk_input.get_value_from_normalized_fcm(i,j) );
            sum_probability+=val;

            // std::cout << "("<< i << "," << j<< ")" << "\t" << mk_target.get_value_from_normalized_fcm(i,j) <<"\t" << mk_input.get_value_from_normalized_fcm(i,j) << "\t" <<  val <<"\t" << sum_probability<<std::endl;
            
        }
    }
    return sum_probability/c_size;
}

double KullbackLeiblerDivergency::compute_divergency_p_k_elem(MarkovTable &mk_input, MarkovTable &mk_target) const{
    unsigned int c_size= pow(mk_input.alphSz,mk_input.get_context());
    unsigned int alphabet_size= mk_input.alphSz;

    double sum_probability= 0;
    auto sum_all_elem_target = mk_target.sum_all_elem();
    auto sum_all_elem_input = mk_input.sum_all_elem();

    for (auto i=0u; i<c_size;++i){
        for (auto j=0u; j<alphabet_size;++j){
            auto p_target=(mk_target.get_value(i,j)+args.lambda)/(sum_all_elem_target + mk_target.get_vector().size()*args.lambda);
            auto p_input= (mk_input.get_value(i,j)+args.lambda)/(sum_all_elem_input + mk_input.get_vector().size()*args.lambda);
            sum_probability+= p_target * log2( p_target/p_input);
        }
    }
    return sum_probability/c_size;
}

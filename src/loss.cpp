#include "loss.h"


struct loss_weight_exception: public std::exception {
    using std::exception::what;
    virtual const char* what() {
        return "invalid loss weight input received";
    }
};


Loss::Loss(Args &args, double &weight, std::vector<MarkovTable> &mkv_table_vector):w(weight),kl(args){
    if(w>1 | w<0){
        throw loss_weight_exception{};
    }
}

double Loss::compute_loss(std::vector<MarkovTable> &mkv_table_vector){
    double conditional_divergence=0;
    double seq_string_divergence=0;
    
    if(w==1){
        auto conditional_divergence_vector = kl.compute_divergency_pconditional(mkv_table_vector);
        conditional_divergence = average_divergence_vector(conditional_divergence_vector);
        return conditional_divergence;
    }
    else if(w==0){
        auto seq_string_divergence_vector = kl.compute_divergency_p_k_elem(mkv_table_vector);
        seq_string_divergence = average_divergence_vector(seq_string_divergence_vector);
        return seq_string_divergence;
    }
    else{
        auto conditional_divergence_vector = kl.compute_divergency_pconditional(mkv_table_vector);
        auto seq_string_divergence_vector = kl.compute_divergency_p_k_elem(mkv_table_vector);
        seq_string_divergence = average_divergence_vector(seq_string_divergence_vector);
        conditional_divergence = average_divergence_vector(conditional_divergence_vector);
        return (conditional_divergence*w)+(seq_string_divergence*(1-w));
    }
}

double Loss::average_divergence_vector(std::vector<double> &divergence_vector){
    if(divergence_vector.empty()){
        return 0;
    }
    auto const count = static_cast<float>(divergence_vector.size());
    return std::reduce(divergence_vector.begin(), divergence_vector.end()) / count;
}

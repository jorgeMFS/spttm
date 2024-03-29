#include "loss.h"


struct loss_weight_exception: public std::exception {
    using std::exception::what;
    virtual const char* what() {
        return "invalid loss weight input received";
    }
};

// tm_bool in kl was set to true since we intend to use loss in TMs
Loss::Loss(Args &args, double &weight):w(weight),kl(args){
    if(w>1 || w<0){
        throw loss_weight_exception{};
    }
}

double Loss::length_penalizer(MarkovTable &mkv_table){
    auto input_length = mkv_table.sum_all_elem();
    auto targer_lenght = kl.target_lenght;
    
    if (input_length>=targer_lenght){
        return -log2(targer_lenght/static_cast<double>(input_length)); // 0.0001
    }else{
        return -log2(input_length/static_cast<double>(targer_lenght));
    }
    //return -log2(input_length/targer_lenght) - log2(targer_lenght/input_length);

}

double Loss::compute_loss(std::vector<MarkovTable> &mkv_table_vector){
    double conditional_divergence=0;
    double seq_string_divergence=0;
    
    auto length_pen = 0.01 * length_penalizer(mkv_table_vector[0]);
    //length_pen = 0.01 * length_pen*log2(length_pen);
    auto NORMALIZER_TRANSFORMATION = [](unsigned int x){return static_cast<double>(x);};
    //auto NORMALIZER_TRANSFORMATION = [](unsigned int x){return pow(x, 2);};
    //auto NORMALIZER_TRANSFORMATION = [](unsigned int x){return x*log2(x+1);};
    if(w==1){
        auto conditional_divergence_vector = kl.compute_divergency_pconditional(mkv_table_vector, NORMALIZER_TRANSFORMATION);
        conditional_divergence = average_divergence_vector(conditional_divergence_vector);
        return conditional_divergence + length_pen;
    }
    else if(w==0){
        auto seq_string_divergence_vector = kl.compute_divergency_p_k_elem(mkv_table_vector, NORMALIZER_TRANSFORMATION);
        seq_string_divergence = average_divergence_vector(seq_string_divergence_vector);
        return seq_string_divergence + length_pen;
    }
    else{
        auto conditional_divergence_vector = kl.compute_divergency_pconditional(mkv_table_vector, NORMALIZER_TRANSFORMATION);
        auto seq_string_divergence_vector = kl.compute_divergency_p_k_elem(mkv_table_vector, NORMALIZER_TRANSFORMATION);

        seq_string_divergence = average_divergence_vector(seq_string_divergence_vector);
        conditional_divergence = average_divergence_vector(conditional_divergence_vector);
        // add penalty when lenght of the tapes differ

        //std::cerr << conditional_divergence*0.5 << " seq: " << seq_string_divergence*0.5 << " lenP: " << 0.01*length_penalizer(mkv_table_vector[0]) << std::endl;
        // fix the manual 0.1
        return (conditional_divergence*w)+(seq_string_divergence*(1-w)) + length_pen;
    }
}

double Loss::average_divergence_vector(std::vector<double> &divergence_vector){
    if(divergence_vector.empty()){
        return 0;
    }
    auto const count = static_cast<float>(divergence_vector.size());
    return std::reduce(divergence_vector.begin(), divergence_vector.end()) / count;
}

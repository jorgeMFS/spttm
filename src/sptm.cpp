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
#include <utility>
#include <vector>


sptm::sptm(Args args):str(args.input_file,args.alphabet_size){
    
    StateMatrix st(args.states, args.alphabet_size);
    std::minstd_rand rng{args.sd};
    input_amplitude = str.get_input_vector().size();
    st.set_random(rng);
    TuringMachine tm(st);
    MarkovTable mrkvTable(args.k[0], args.alphabet_size, args.alpha);
    InteractiveMarkovModel model(args.k[0], args.alphabet_size, args.alpha);
    
    
    TapeMoves tpMove;
    for (auto i = 0u; i < args.tape_iterations; ++i){
        tpMove = tm.act(); // grave esti antaŭe
        model.update_table(tpMove, tm.turingTape);
        if(i%10==0){
            auto written_tape = tm.get_tape().get_tape_vector(0);
            auto mdl = model.get_markov_table();
            
            auto tape_length = tm.get_tape_size();
            auto diff = get_diff_amplitudes(tape_length);    
            
            auto nrc_vector = this->str.readinput(mdl);
            index = std::min_element(nrc_vector.begin(), nrc_vector.end()) - nrc_vector.begin();
            
            per_elem_input = percentage_elements(str.get_transcribed_vector(index), str.cardinality());
            auto per_elem_tm = percentage_elements(written_tape, str.cardinality());
            auto elem_dist_diff = perc_diff_elems(per_elem_input,per_elem_tm);
            auto ht_dst = hit_score_and_dist(str.get_transcribed_vector(index), written_tape);
            
            std::cout << nrc_vector[index] << "\t" << diff << "\t" << ht_dst.first << "\t" << ht_dst.second << "\t";
            for (auto &x : elem_dist_diff){
                std::cout << x << "\t";
            }
            std::cout << std::endl;
        }

    }

    std::cout << "@" <<std::endl;

    while(true){
        sptm::update();
    }
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
        if (i%10==0){
            auto written_tape = tm.get_tape().get_tape_vector(0);
            auto tape_length = tm.get_tape_size();
            auto diff = get_diff_amplitudes(tape_length);
            auto mdl = model.get_markov_table();
            auto nrc_value = this->str.readinput(mdl,index);
            auto ht_dst = hit_score_and_dist(str.get_transcribed_vector(index), written_tape);
            std::cout << nrc_value << "\t" << diff << "\t" << ht_dst.first << "\t" << ht_dst.second << "\t";
            auto per_elem_tm = percentage_elements(written_tape, str.cardinality());
            auto elem_dist_diff = perc_diff_elems(per_elem_input,per_elem_tm);
            for (auto &x : elem_dist_diff){
                std::cout << x << "\t";
            }
            std::cout << std::endl;
        }
    }
     std::cout << "@" <<std::endl;
}; 

std::tuple<StateMatrix, unsigned long long, unsigned int, double, unsigned int> read_file() {
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
    return candidate;
};

double sptm::get_diff_amplitudes(unsigned int & tape_length){
    auto diff = std::abs(static_cast<double>(tape_length)-static_cast<double>(input_amplitude));
    auto max = std::max(static_cast<double>(tape_length),static_cast<double>(input_amplitude));
    return diff/max;
}

std::vector<double> sptm::percentage_elements(std::vector<unsigned int> const & written_tape,  int const & alphabet_size){
    std::unordered_map<unsigned, size_t> counts;
    for (auto v : written_tape)
        ++counts[v];
    
    std::vector<double> elem_percentages;
    for (int i=0; i<alphabet_size; ++i){
        if (counts.find(i) == counts.end()){
            elem_percentages.push_back(0);
        }
        else{
            elem_percentages.push_back(static_cast<double>(counts[i])/static_cast<double>(written_tape.size()));
        }
    }
    return elem_percentages;
}

std::vector<double> sptm::perc_diff_elems(std::vector<double> const & v1, std::vector<double> const & v2){
    
    std::vector<double> result;
    std::vector<double> result2;
    std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(result),std::minus<double>());
    
    for (auto x:result){
        if(x<0){ x=x*-1;}
        result2.push_back(x);
    }
    
    return result2;
}


template <typename T>
double	vectors_distance(const std::vector<T>& a, const std::vector<T>& b)
{
	std::vector<double>	auxiliary;

	std::transform (a.begin(), a.end(), b.begin(), std::back_inserter(auxiliary),//
	[](T element1, T element2) {return pow((element1-element2),2);});
	auxiliary.shrink_to_fit();

	return  std::sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
} 


std::pair<double, double> sptm::hit_score_and_dist(std::vector<unsigned int> const & v1, std::vector<unsigned int> const & v2){
    std::vector<unsigned int> v1_n;
    std::vector<unsigned int> v2_n;

    if (v1.size()> v2.size()){
        auto diff = v1.size() - v2.size();
        v1_n = std::vector<unsigned int>(v1.begin() , v1.end()-diff);
        v2_n = v2;
    }
    else{
        auto diff = v1.size() - v2.size();
        v1_n = v1;
        v2_n = std::vector<unsigned int>(v2.begin() , v2.end()-diff);
    }
        hit_score(v1_n, v2_n);
    return std::pair<double, double> (hit_score(v1_n, v2_n),vectors_distance(v1_n, v2_n));
}


double hit_score(std::vector<unsigned int> const & v1, std::vector<unsigned int> const & v2){
    auto counter=0;
    for(long unsigned int x=0; x<v1.size();++x){
        if (v1[x]!=v2[x]){
            ++counter;
        }
    }
    return static_cast<double>(counter)/v1.size();
}
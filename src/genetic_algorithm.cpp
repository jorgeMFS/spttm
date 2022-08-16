// This is it, it's the do or die
#include <iostream>
#include <string>
#include <sys/random.h> //test if it works
#include "util.h"
#include "genetic_algorithm.h"
#include "turingMachine.h"
#include "serialization.h"

bool compare_pair_NRC::operator()(const std::pair<std::string, double> & n1, const std::pair<std::string, double> & n2){
    if (n1.second == n2.second)
    {
      return n1.first < n2.first;
    }
    return n1.second < n2.second;
}

bool compare_pair_nc::operator()(const std::pair<std::string, double> & n1, const std::pair<std::string, double> & n2){
    if (n1.second == n2.second)
    {
      return n1.first < n2.first;
    }
    return n1.second > n2.second;
}


GeneticTM::GeneticTM(Args args){
    tape_iterations(args.tape_iterations), vector_k(vector_k), seed(sd), alph_sz(rule_matrix.alphSz),
                    nb_states(rule_matrix.nbStates),
}
GeneticTM::GeneticTM(StateMatrix rule_matrix,
                    unsigned int tape_it,
                    std::vector<unsigned int> vector_k,
                    const double alpha,
                    StringProcess strProcess,
                    const unsigned int sd,
                    const unsigned int tm_it,
                    const unsigned int jobs,
                    const double min_f,
                    const double alpha_nc=0.05,
                    const unsigned int min_tape_sz=200,
                    const unsigned int max_tape_sz=10000,
                    const double  nc_threshold=0.95,
                    const unsigned int init_sons=50):
    
                    cost_calculator(rule_matrix, tape_it, vector_k[0], strProcess, alpha,min_tape_sz, 0.1),
                    tape_iterations(tape_it), vector_k(vector_k), seed(sd),
                    search_it(tm_it), jobs(jobs), f_min(min_f), alph_sz(rule_matrix.alphSz),
                    nb_states(rule_matrix.nbStates), init_sons(init_sons), min_tape_sz(min_tape_sz), max_tape_sz(max_tape_sz),
                    tm_imp(alph_sz,nb_states, vector_k, alpha_nc, init_sons, jobs, min_tape_sz, max_tape_sz, nc_threshold, tape_iterations, seed){
        
        genetic_increase();
    }

double GeneticTM::cost_calculator(){
    //TODO
}

void GeneticTM::genetic_increase(){
    
    std::minstd_rand rng{this->seed}; //or get random seed
    unsigned int top_n = 300;
    auto nsons=init_sons;
    // auto number_of_initial_machines=100;
    // create_initial_pool(number_of_initial_machines);
    read_from_file("NC_complex_list");
    //input rule matrix
    StateMatrix st_m(nb_states, alph_sz);
    double new_value=3;
    while(open_set_top().second>f_min){
        //for each in list mutate n times and append to open_set
        
        std::vector<StateMatrix> sons =  get_top_n_del_rest(top_n);
        st_m.set_random(rng);
        
        // auto complex_tm_pair = tm_imp.retrive_complex();
        // StateMatrix st_m(complex_tm_pair.first, nb_states, alph_sz);
        for (auto &st:sons){
            create_sons(st, st_m, nsons);
            nsons-=nsons/20;
        }

        if (new_value>open_set_top().second){
            auto b =0;
            for (auto &x:open_set){
                set_output("current NRC", x.second);
                std::cerr<< bold_on << cyan_on << "RM: "<< bold_off <<bold_on << x.first << bold_off << std::endl;
                ++b;
                if(b>5){
                    break;
                }
            }
            draw_line();
            new_value=open_set_top().second;
        }
        
        // extinction_event(10,0.70);
        extinction_event(50,0.772);
        // tape_iterations+=50; --> the cost is curently tape_it fixed... 
    }   
        StateMatrix st(open_set_top_pop().first,nb_states,alph_sz);
        Serialization sr(st, tape_iterations, vector_k[0], seed, "Programs found");
}


void GeneticTM::create_sons(StateMatrix rm1, StateMatrix sm_m, const unsigned int & number_of_sons){
    auto size = open_set.size();
    auto final_size = size + number_of_sons;
    std::set<std::string> rd;
    while(size<final_size){
        StateMatrix son = rm1.create_son(sm_m);
        auto pair = rd.emplace(son.get_state_matrix_string());
        if(pair.second){
            if(cost_calculator.symbol_frequency_similarity(son)){
                auto cost=cost_calculator.determine_cost(son);
                open_set.emplace(std::pair<std::string, double>(son.get_state_matrix_string(),cost));
                ++size;
            }
        }
    } 
 }

void GeneticTM::create_initial_pool(const unsigned int & n){
    std::set<std::string> sts;
    while (open_set.size()< n){
        auto nc_pair = tm_imp.retrive_complex();   
        if (sts.emplace(nc_pair.first).second){
             StateMatrix statematrix(nc_pair.first,nb_states,alph_sz);
             auto cost=cost_calculator.determine_cost(statematrix);
             open_set.emplace(nc_pair.first,cost);
        }
    }
}

void GeneticTM::read_from_file(std::string filename="NC_complex_list"){
    open_set.clear();
    Deserialization dsr(filename);
    auto candidates = dsr.get_all_candidates();
    for (auto &candidate:candidates){
        StateMatrix target_rule_matrix = std::get<0>(candidate);
        double cost=cost_calculator.determine_cost(target_rule_matrix);
        std::pair<std::string,double> candidate_pair(target_rule_matrix.get_state_matrix_string(),cost);
        open_set.emplace(candidate_pair);
    }
}

void GeneticTM::extinction_event(unsigned int index_range, double extinction_threshold=0.775){
    unsigned int counter=0;
    for (auto n = 0u; n<index_range-1;++n){
        StateMatrix top_st(open_set_get(n).first, nb_states, alph_sz);
        for (auto i=n+1; i<index_range;++i){
            auto elem = open_set_get(i);
            // closed_set.emplace(elem.first);
            StateMatrix st(elem.first, nb_states, alph_sz);
            if (top_st.similarity(st)>=extinction_threshold){
                open_set_remove(i);
                ++counter;
            }
        } 
    }  
    if(counter>20){
    ++seed;
    }
}


std::vector<StateMatrix> GeneticTM::get_top_n_del_rest(unsigned int & top_n){
    if (top_n>open_set.size()){
        top_n=open_set.size();
    }
    std::vector<StateMatrix> top_st;
    auto it = open_set.begin();

    for (auto i = 0u; i < top_n; ++i){
        auto elem = *it++;
        StateMatrix st(elem.first,nb_states,alph_sz);
        top_st.push_back(st);
    }
    open_set.erase(it,open_set.end());
    return top_st;
}

void GeneticTM::open_set_remove(unsigned int index){
    auto it = open_set.begin();
    std::advance(it, index);
    open_set.erase(it);
}

std::pair<std::string, double> GeneticTM::open_set_top_pop(){
    auto i = *open_set.begin();
    open_set.erase(open_set.begin());
    return i; 
}

std::pair<std::string, double> GeneticTM::open_set_top(){
    auto i = *open_set.begin();
    return i; 
}

std::pair<std::string, double> GeneticTM::open_set_get(unsigned int index){
    auto it = open_set.begin();
    std::advance(it, index);
    auto i = *it;
    return i; 
}

void GeneticTM::print_open_set() const{
    for (auto elem:open_set){
        set_output("current NRC", elem.second);
        std::cerr<< bold_on << cyan_on << "RM: "<< bold_off << bold_on << elem.first << bold_off << std::endl;
        draw_line();
    } 

}
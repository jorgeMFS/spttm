/**
    sptm.cpp
    Purpose: Get inputs parameters, output metrics.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <fstream>
#include <string>

#include "interactiveMarkovModel.h"
#include "tprl.h"
#include "turingMachine.h"
#include"util.h"


tprl::tprl(Args args, bool print_all_bool):print_all_bool(print_all_bool),counter(0),st(args.states, args.alphabet_size),rng{args.sd}{
    std::ofstream outFile(args.input_file+"_"+std::to_string(counter));
    auto i = 0u;
    num_it = static_cast<double>(args.tape_iterations) / static_cast<double>(args.num_out_lines);
    if(print_all_bool){
        while (i++ < args.n_files){
            step(args);
        }
    }
    else{
        while (i++ < args.n_files){
            step_2(args);
        }
    }
}


void tprl::step(Args args){
    std::string file = "./dataset/"+args.input_file+"_"+std::to_string(++counter);
    std::ofstream outFile(file);
    
    TapeMoves tpMove;
    std::vector<unsigned int> recordM;
    std::vector<size_t> index_list;
    std::vector<std::vector<Char>> tapes;
    st.set_random(rng);
    TuringMachine tm(st);
   
    TMCycleDetector TMCycleDetector;
    bool b=true;
    

    do {
        if(detector_counter<ULLONG_MAX){
            if((++detector_counter%1000==0) & (less_dect < 80)) {
                ++less_dect;
            }
        }
        st.set_random(rng);
        st.print_st_matrix_vector();
        tm.set_tm(st);
        b = TMCycleDetector.cycledetector(tm,  100-less_dect);
    }
    while(b==true);
    for (auto i = 0u; i < args.tape_iterations; ++i){
        auto tuple_tp_rule = tm.act_rule(); // grave esti antaŭe
        tpMove = std::get<0>(tuple_tp_rule);
        recordM.push_back(std::get<1>(tuple_tp_rule));
        tapes.push_back(tm.get_tape().get_tape_vector(0));
        index_list.push_back(std::get<2>(tuple_tp_rule));
    }

    auto st_m  = st.print_st_matrix_vector();
    outFile << st_m;
    outFile  << std::endl;
    for(const auto &e : recordM){
            outFile  << e << ",";
    }
    outFile  << std::endl;
    for (const auto &tp : tapes){
        for (const auto &el: tp){
            outFile  << el;
        }
        outFile  << ";";
    }
    outFile  << std::endl;
    for(const auto &i : index_list){
            outFile  << i << ",";
    }
}

void tprl::step_2(Args args){
    std::string file = "./tapeTestPool/"+args.input_file+"_"+std::to_string(++counter);
    std::ofstream outFile(file);
    
    std::vector<std::vector<Char>> tapes;
    st.set_random(rng);
    TuringMachine tm(st);
   
    bool new_entry;
    TMCycleDetector TMCycleDetector;
    bool b= true;
    
    do {
        if(detector_counter<ULLONG_MAX){
            if((++detector_counter%1000==0) & (less_dect < 80)) {
                ++less_dect;
            }
        }
        new_entry = false;
        st.set_random(rng);
        tm.set_tm(st);
        b = TMCycleDetector.cycledetector(tm, 100-less_dect);
        if (b==false){
            tm.reset_tape_and_state();
            for (auto i = 0u; i < args.tape_iterations; ++i){
                tm.act_rule(); // grave esti antaŭe
            }
            new_entry = tm_created.insert(tm.get_tape().print_written_tape(false)).second;
        }
    }
    while(new_entry==false);

    auto tp = tm.get_tape().get_tape_vector(0);
    std::cerr << bold_on << green_on << file <<": "<< bold_off << red_on  << st.get_state_matrix_string() << bold_off << std::endl;
    for (const auto &el: tp){
        outFile  << el;
    }
}


    
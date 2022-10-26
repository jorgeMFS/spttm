/**
    sptm.cpp
    Purpose: Get inputs parameters, output metrics.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

#include "interactiveMarkovModel.h"
#include "nc.h"
#include "tprl.h"
#include "turingMachine.h"
#include"util.h"


tprl::tprl(Args args, bool print_all_bool):print_all_bool(print_all_bool),counter(0),st(args.states, args.alphabet_size),rng{args.sd}{
    unsigned long long number_iterations=0;
    double nc_min=0.0;
    bool increase_nc=false;
    auto i = 0u;
    num_it = static_cast<double>(args.tape_iterations) / static_cast<double>(args.num_out_lines);

    if(print_all_bool){
        while (++i <= args.n_files + 1){
            step(args);
        }
    }
    else{
        std::unordered_set<std::string> tm_created;
        auto cnt=0u;
        auto files_per_slot=args.n_files/10;
        unsigned long long value_division=args.traversal_len / 10;
        while ( (i <= args.n_files) && (number_iterations<=args.traversal_len) && (nc_min<=0.8)  ){
            step_2(args, number_iterations, nc_min, increase_nc, value_division, tm_created);
            if ((++cnt>=files_per_slot) | increase_nc){
                nc_min+=0.2;
                tm_created.clear();
                files_per_slot+=args.n_files/10;
                increase_nc=false;
            }
        }
    }
}



void tprl::step(Args args){
    auto alphabet_subfolder=std::to_string(args.alphabet_size);
    auto state_subfolder=std::to_string(args.states);
    std::string path = "dataset/"+alphabet_subfolder+"/"+state_subfolder+"/";
    
    
    TapeMoves tpMove;
    std::vector<unsigned int> recordM;
    std::vector<size_t> index_list;
    std::vector<std::vector<Char>> tapes;
    st.set_random(rng);
    TuringMachine tm(st);
   
    TMCycleDetector TMCycleDetector;
    bool b=true;
    

    do {

        st.set_random(rng);
        st.print_st_matrix_vector();
        tm.set_tm(st);
        b = TMCycleDetector.cycledetector(tm,  20);
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
    std::string file = path + args.input_file+"_"+std::to_string(++counter);
    std::filesystem::create_directories(path);
    std::ofstream outFile(file);
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

void tprl::step_2(Args args, unsigned long long &number_iterations, double &nc_min, bool &increase_nc, unsigned long long & value_division, std::unordered_set<std::string> &tm_created){
    auto alphabet_subfolder=std::to_string(args.alphabet_size);
    auto state_subfolder=std::to_string(args.states);
    auto val=nc_min*10.0;
    auto nc_subfolder=std::to_string(static_cast<int>(std::round(val)));
    std::string path = "tapeTestPool/"+alphabet_subfolder+"/"+state_subfolder+"/"+nc_subfolder+"/";
    std::vector<std::vector<Char>> tapes;
    
    st.set_random(rng);
    TuringMachine tm(st);
    bool new_entry;
    
    TMCycleDetector TMCycleDetector;
    bool b=true;
    increase_nc=false;
    NormalizedCompressionMulipleContexts<NormalizedCompressionMarkovTable> ncMarkovTables(args.k, args.alphabet_size, args.alpha);

    do {
        new_entry = false;
        st.set_random(rng);
        tm.set_tm(st);
        b = TMCycleDetector.cycledetector(tm, 10);
        if (b==false){
            tm.reset_tape_and_state();
            for (auto i = 0u; i < args.tape_iterations; ++i){
                tm.act_rule(); // grave esti antaŭe
            }
            auto tape = tm.get_tape();
            Metrics metrics = ncMarkovTables.update_get_metrics_min_nc(tape);
            ncMarkovTables.reset();
            if(metrics.normalizedCompression > nc_min && metrics.amplitude>100 && metrics.normalizedCompression < (nc_min+0.2) ){
                new_entry = tm_created.insert(tm.get_tape().print_written_tape(false)).second;
            }
        }
        
        if( ++number_iterations > value_division ){value_division += args.traversal_len / 10; increase_nc=true; return;}
    }
    while(new_entry==false);
    std::string file = path + args.input_file+"_"+std::to_string(++counter);
    
    auto tp = tm.get_tape().get_tape_vector(0);
    std::cerr << bold_on << green_on << file <<": "<< bold_off << red_on  << st.get_state_matrix_string() <<bold_off << std::endl;
    std::filesystem::create_directories(path);
    std::ofstream outFile(file);
    for (const auto &el: tp){
        outFile  << el;
    }
}


    
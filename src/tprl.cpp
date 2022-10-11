/**
    sptm.cpp
    Purpose: Get inputs parameters, output metrics.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include "tprl.h"
#include "interactiveMarkovModel.h"
#include "turingMachine.h"
#include <fstream>
#include <string>


tprl::tprl(Args args):counter(0),st(args.states, args.alphabet_size),rng{args.sd}{
    std::ofstream outFile(args.input_file+"_"+std::to_string(counter));
    auto i = 0u;
    num_it = static_cast<double>(args.tape_iterations) / static_cast<double>(args.num_out_lines);
    while (i++ < args.n_files){
        step(args);
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
    auto b = TMCycleDetector.cycledetector(tm, 100);
    
    do {
        st.set_random(rng);
        st.print_st_matrix_vector();
        tm.set_tm(st);
        b = TMCycleDetector.cycledetector(tm, 50);
    }
    while(b==true);
    
    //get NRC or cycle.
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
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
    num_it = static_cast<double>(args.tape_iterations) / static_cast<double>(args.num_out_lines);
    while(true){
        step(args);
        exit(44);
    }
}

void tprl::step(Args args){
    std::string file = "./dataset/"+args.input_file+"_"+std::to_string(++counter);
    std::ofstream outFile(file);
    
    TapeMoves tpMove;
    std::vector<unsigned int> recordM;
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
        auto pair_tp_rule = tm.act_rule(); // grave esti antaŭe
        tpMove = pair_tp_rule.first;
        recordM.push_back(pair_tp_rule.second);
        tapes.push_back(tm.get_tape().get_tape_vector(0));
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
}
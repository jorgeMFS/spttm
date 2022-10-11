#include "spark.h"
#include "turingMachine.h"
#include "markovTable.h"
#include "interactiveMarkovModel.h"
#include "read_input.h"
#include <iterator>
#include <sstream>
#include <utility>
#include <vector>
#include <cassert>

spark::spark(Args args):str(args.input_file,args.alphabet_size){
    init(args);
}


void spark::init(Args args){
    num_it = static_cast<double>(args.tape_iterations) / static_cast<double>(args.num_out_lines);
    StateMatrix st(args.states, args.alphabet_size);
    std::minstd_rand rng{args.sd};
    input_amplitude = str.get_input_vector().size();
    st.set_random(rng);
    TuringMachine tm(st);
    InteractiveMarkovModel model(args.k[0], args.alphabet_size, args.alpha);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
    TapeMoves tpMove;
    for (auto i = 0u; i < args.tape_iterations; ++i){
        tpMove = tm.act(); // grave esti antaŭe
        model.update_table(tpMove, tm.turingTape);
        all_models.update_tables(tpMove, tm.turingTape);

        if(i%num_it==0){
            auto mdl = model.get_markov_table();
            auto mdls = all_models.get_markov_tables();
            auto written_tape = tm.get_tape().get_tape_vector(0);
            auto tape_length = tm.get_tape_size();
            auto diff = get_diff_amplitudes(tape_length);    
            auto nrc_vector = this->str.readinput(mdl);
            index = std::min_element(nrc_vector.begin(), nrc_vector.end()) - nrc_vector.begin();
            auto avg_nrc = this->str.get_average_nrc(mdls,index);
            std::cerr << "amplitude discrepancy => "<< diff << " units" << std::endl;
            std::cerr << "avg nrc: "<< avg_nrc << std::endl;
            exit(1);
        }
    }

    std::cout << "@" <<std::endl;

    // while(true){
    //     sptm::update();
    // }
}



double spark::get_diff_amplitudes(unsigned int & tape_length){
    auto diff = std::abs(static_cast<double>(tape_length)-static_cast<double>(input_amplitude));
    auto max = std::max(static_cast<double>(tape_length),static_cast<double>(input_amplitude));
    return diff/max;
}
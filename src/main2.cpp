#include "sptm.h"
#include "interactiveMarkovModel.h"

int main (int argc, char **argv){
    auto input=read_file();
    auto st = std::get<0>(input);
    auto number_iterations = std::get<1>(input);
    auto k = std::get<2>(input);
    auto alpha = std::get<3>(input); 

    TuringMachine tm(st);
    TapeMoves tpMove;
    for (auto i = 0u; i < number_iterations; ++i){
        tpMove = tm.act(); // grave esti antaŭe
    }
    tm.print_written_tape(true);
    return 1;
}
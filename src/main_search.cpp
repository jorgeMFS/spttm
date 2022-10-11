/**
    main_search.cpp
    Purpose: Run Basic Search Program
            1) Receives input to search;
            2) 2 Flags (sequential search or montecarlo search) and threshold.
            3) Searchs space using NRC (various k).
            4) When threshold is obtained stops.
            5) Outputs best results.
    
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <stdexcept>
#include "parseArgs.h"


int main (int argc, char **argv){

    Args argument = parseArgs(argc,argv);

    printArgs(argument);
    auto strategy = argument.search_strategy;
    if (std::strcmp(strategy, "Null") == 0){
        throw std::invalid_argument("search_strategy argument can not be null in this program");
    }

  // spark spark(argument);
  // spark.update();
  
}

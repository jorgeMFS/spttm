
#include "sequentialSearch.h"




SequentialSearch::SequentialSearch(){
    init();
}



/** Evaluate all relative Turing machine programs with the given architecture.
 *
 * @param states
 * @param alphabet_size
 * @param num_iterations
 * @param kvector vector of values of k
 * @param strategy the Turing machine traversal strategy
 * @param traversal_len number of different Turing machines to run, can be 0 in
 *        sequential traversal for the full TM domain
 * @param traversal_offset offset of the partition to travers (only in sequential strategy)
 * @param verbose
 * @return a struct containing the results of evaluation, one per Turing machine
*/



CompressionResultsData find_tm(
    size_t states,
    size_t alphabet_size,
    unsigned int num_iterations,
    const std::vector <unsigned int>& kVector,
    std::pair <double,bool> threshold,
    StringProcess strProcess,
    TraversalStrategy strategy,
    unsigned long long traversal_len,
    unsigned long long traversal_offset,
    bool verbose) {
      
    // Turing Machine
    TuringMachine machine(states, alphabet_size);
    CompressionResultsData data;
    data.clear_data();
    BestKMarkovTables<NormalizedCompressionMarkovTable> bestMkvTableCompression(kVector, alphabet_size);
    InteractiveMarkovModel iteractiveMarkov(kVector.front(), alphabet_size); // change this for multiple k

    switch (strategy) {
        case TraversalStrategy::SEQUENTIAL: {
            if (traversal_offset > 0) {
                machine.stMatrix.set_by_index(traversal_offset);
            }
            if (traversal_len == 0) {
                traversal_len = tmCardinality(states, alphabet_size);
            }
            unsigned int counter = 0;
            do {
                auto indAndmetrics = test_machine(machine, 
                                        bestMkvTableCompression,
                                        iteractiveMarkov,
                                        num_iterations,
                                        strProcess, 
                                        threshold.first);
                
                if (verbose && counter % 4096 == 0) {
                    std::cerr << "TM #" << std::setw(8) << indAndmetrics.tmNumber << ": amplitude = " << indAndmetrics.metrics.amplitude 
                    << ": sc = " << std::setprecision(5) << std::showpoint <<indAndmetrics.metrics.selfCompression <<": nc = " << std::setprecision(5) << std::showpoint 
                    << indAndmetrics.metrics.normalizedCompression << "\r";
                }

                if(indAndmetrics.notZero()) {
                  data.append_metrics(indAndmetrics);
                  
                } 

                machine.stMatrix.next();
                counter += 1;
            } while (counter < traversal_len);
        }
        break;
        case TraversalStrategy::MONTE_CARLO: {
            // initialize random number generator
            std::random_device rnd_device;
            Rng rng{rnd_device()};

            for (auto counter = 0ull; counter < traversal_len; counter++) {
                machine.stMatrix.set_random(rng);
                //machine.stMatrix.print();
                auto indAndmetrics = test_machine(machine, 
                                        bestMkvTableCompression,
                                        iteractiveMarkov,
                                        num_iterations,
                                        strProcess, 
                                        threshold.first);
                if (verbose && counter % 4096 == 0) {
                std::cerr << "TM #" << std::setw(8) << indAndmetrics.tmNumber << ": amplitude = " << indAndmetrics.metrics.amplitude 
                << ": sc = " << std::setprecision(5) << std::showpoint <<indAndmetrics.metrics.selfCompression <<": nc = " << std::setprecision(5) << std::showpoint
                << indAndmetrics.metrics.normalizedCompression << "\r";
                }
                
                // get Turing machine index
                 if(indAndmetrics.notZero()) {
                    data.append_metrics(indAndmetrics);
                }   
            }
        }
        break;
    }

    if (verbose) {
        std::cerr << std::endl;
    }
    return data;
}
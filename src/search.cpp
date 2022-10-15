
#include "search.h"
#include "util.h"

Search::Search(Args args, double weight): args(args),loss(args,weight),traversal_len(args.traversal_len){
    if (args.traversal_len==0){
        traversal_len=tm_cardinality(args.states, args.alphabet_size);
    }
    init();
}

void Search::init(){

    if (std::strcmp(args.search_strategy, "Null") == 0){
        throw std::invalid_argument("search_strategy argument can not be null in this program");
    }
    else if (std::strcmp(args.search_strategy, "Sequential")== 0){
        SequentialSearch();
    }
    else if(std::strcmp(args.search_strategy, "Monte_Carlo") == 0){
        MonteCarloSearch();
    } 
    else if(std::strcmp(args.search_strategy, "Neural_Networks") == 0){
        std::cout << "ERROR ERROR" << std::endl;
        exit(-1); // fix latter
    }
}

void Search::SequentialSearch(){

    //stored data;
    std::vector<std::pair<StateMatrix, double>> tm_data;
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
    StateMatrix st(args.states,args.alphabet_size);
    
    TmId counter = 0;
    do {
        double loss = test_machine(st,all_models);
        if(loss<args.threshold) {
            std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
            tm_data.push_back(std::pair<StateMatrix, double>(st, loss));
        } 
        st.next();
        counter += 1;
    } while (counter < traversal_len);
}

double Search::test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models) {
    
    TuringMachine tm(st);
    all_models.reset();
    for (auto i = 0u; i < args.tape_iterations -1 ; ++i){
        TapeMoves tpMove = tm.act(); // grave esti antaŭe
        all_models.update_tables(tpMove, tm.turingTape);
    }
    std::vector<MarkovTable> mkv_vector=all_models.get_markov_tables();
    double tm_loss = loss.compute_loss(mkv_vector);
    return tm_loss;
}

void Search::MonteCarloSearch(){
    std::vector<std::pair<StateMatrix, double>> tm_data;
    StateMatrix st(args.states,args.alphabet_size);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
    std::random_device rnd_device;
    std::minstd_rand rng{args.sd};
    for (auto counter = 0ull; counter < traversal_len; counter++) {
        st.set_random(rng);
        double loss = test_machine(st,all_models);
        if(loss<args.threshold) {
            std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
            tm_data.push_back(std::pair<StateMatrix, double>(st, loss));
        } 
    }
}
// case TraversalStrategy::MONTE_CARLO: {
//             // initialize random number generator
//             std::random_device rnd_device;
//             Rng rng{rnd_device()};

//             for (auto counter = 0ull; counter < traversal_len; counter++) {
//                 machine.stMatrix.set_random(rng);
//                 //machine.stMatrix.print();
//                 auto indAndmetrics = test_machine(machine, 
//                                         bestMkvTableCompression,
//                                         iteractiveMarkov,
//                                         num_iterations,
//                                         strProcess, 
//                                         threshold.first);
//                 if (verbose && counter % 4096 == 0) {
//                 std::cerr << "TM #" << std::setw(8) << indAndmetrics.tmNumber << ": amplitude = " << indAndmetrics.metrics.amplitude 
//                 << ": sc = " << std::setprecision(5) << std::showpoint <<indAndmetrics.metrics.selfCompression <<": nc = " << std::setprecision(5) << std::showpoint
//                 << indAndmetrics.metrics.normalizedCompression << "\r";
//                 }
                
//                 // get Turing machine index
//                  if(indAndmetrics.notZero()) {
//                     data.append_metrics(indAndmetrics);
//                 }   
//             }
//         }
//         break;




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

// CompressionResultsData find_tm(
//     size_t states,
//     size_t alphabet_size,
//     unsigned int num_iterations,
//     const std::vector <unsigned int>& kVector,
//     std::pair <double,bool> threshold,
//     StringProcess strProcess,
//     TraversalStrategy strategy,
//     unsigned long long traversal_len,
//     unsigned long long traversal_offset,
//     bool verbose) {
      
//     // Turing Machine
//     TuringMachine machine(states, alphabet_size);
//     CompressionResultsData data;
//     data.clear_data();
//     BestKMarkovTables<NormalizedCompressionMarkovTable> bestMkvTableCompression(kVector, alphabet_size);
//     InteractiveMarkovModel iteractiveMarkov(kVector.front(), alphabet_size); // change this for multiple k

//     switch (strategy) {
//         case TraversalStrategy::SEQUENTIAL: {
//             if (traversal_offset > 0) {
//                 machine.stMatrix.set_by_index(traversal_offset);
//             }
//             if (traversal_len == 0) {
//                 traversal_len = tmCardinality(states, alphabet_size);
//             }
//             unsigned int counter = 0;
//             do {
//                 auto indAndmetrics = test_machine(machine, 
//                                         bestMkvTableCompression,
//                                         iteractiveMarkov,
//                                         num_iterations,
//                                         strProcess, 
//                                         threshold.first);
                
//                 if (verbose && counter % 4096 == 0) {
//                     std::cerr << "TM #" << std::setw(8) << indAndmetrics.tmNumber << ": amplitude = " << indAndmetrics.metrics.amplitude 
//                     << ": sc = " << std::setprecision(5) << std::showpoint <<indAndmetrics.metrics.selfCompression <<": nc = " << std::setprecision(5) << std::showpoint 
//                     << indAndmetrics.metrics.normalizedCompression << "\r";
//                 }

//                 if(indAndmetrics.notZero()) {
//                   data.append_metrics(indAndmetrics);            
//                 } 

//                 machine.stMatrix.next();
//                 counter += 1;
//             } while (counter < traversal_len);
//         }
//         break;
//         case TraversalStrategy::MONTE_CARLO: {
//             // initialize random number generator
//             std::random_device rnd_device;
//             Rng rng{rnd_device()};

//             for (auto counter = 0ull; counter < traversal_len; counter++) {
//                 machine.stMatrix.set_random(rng);
//                 //machine.stMatrix.print();
//                 auto indAndmetrics = test_machine(machine, 
//                                         bestMkvTableCompression,
//                                         iteractiveMarkov,
//                                         num_iterations,
//                                         strProcess, 
//                                         threshold.first);
//                 if (verbose && counter % 4096 == 0) {
//                 std::cerr << "TM #" << std::setw(8) << indAndmetrics.tmNumber << ": amplitude = " << indAndmetrics.metrics.amplitude 
//                 << ": sc = " << std::setprecision(5) << std::showpoint <<indAndmetrics.metrics.selfCompression <<": nc = " << std::setprecision(5) << std::showpoint
//                 << indAndmetrics.metrics.normalizedCompression << "\r";
//                 }
                
//                 // get Turing machine index
//                  if(indAndmetrics.notZero()) {
//                     data.append_metrics(indAndmetrics);
//                 }   
//             }
//         }
//         break;
//     }

//     if (verbose) {
//         std::cerr << std::endl;
//     }
//     return data;
// }
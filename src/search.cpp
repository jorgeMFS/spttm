
#include <future>
#include <thread>
#include <unordered_set>

#include "search.h"
#include "util.h"


Search::Search(Args args, double weight): args(args),loss(args,weight),traversal_len(args.traversal_len), seed(args.sd){
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
        SequentialSearchMulticore();

    }
    else if(std::strcmp(args.search_strategy, "Monte_Carlo") == 0){
       MonteCarloSearchMulticore();
    } 
    else if(std::strcmp(args.search_strategy, "Neural_Networks") == 0){
        std::cout << "ERROR ERROR" << std::endl;
        exit(-1); // fix latter
    }
}

std::vector<std::pair<StateMatrix, double>> Search::SequentialSearch(TmId traversal_length, TmId traversal_offset){

    //stored data;
    std::vector<std::pair<StateMatrix, double>> tm_data;
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
    
    StateMatrix st(args.states,args.alphabet_size);
    TmId counter = 0;
    if (traversal_offset > 0) {
                st.set_by_index(traversal_offset);
    }

    do {
        double loss = test_machine(st,all_models);
        if(loss<args.threshold) {
            std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
            tm_data.push_back(std::pair<StateMatrix, double>(st, loss));
            std::cerr<< st.get_state_matrix_string()<<std::endl;
            if(loss==0){
                found_program=true;
                return tm_data;
            }
        } 
        st.next();
        counter += 1;
    } while ((counter < traversal_length) & (found_program==false));
    return tm_data;
}

double Search::test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models) {
    
    TuringMachine tm(st);
    all_models.reset();
    for (auto i = 0u; i < args.tape_iterations ; ++i){
        TapeMoves tpMove = tm.act(); // grave esti antaŭe
        all_models.update_tables(tpMove, tm.turingTape);
    }
    std::vector<MarkovTable> mkv_vector=all_models.get_markov_tables();
    return loss.compute_loss(mkv_vector);
}

std::vector<std::pair<StateMatrix, double>> Search::MonteCarloSearch(TmId traversal_length){
    std::vector<std::pair<StateMatrix, double>> tm_data;
    StateMatrix st(args.states,args.alphabet_size);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);

    std::unordered_set<std::string> st_matrix;
    bool entry=false;

    auto rd_dev = std::random_device{};
    std::seed_seq seq{rd_dev(), rd_dev(), rd_dev(), rd_dev()};
    std::mt19937 rng(seq);

    if (seed>0){
        std::minstd_rand rng{seed};
    }

    for (auto counter = 0ull; counter < traversal_length; counter++) {
        if(found_program){
            return tm_data;
        }

        do{
            st.set_random(rng);
            entry = st_matrix.insert(st.get_state_matrix_string()).second;
        }
        while(entry==false);
        
        
        double loss = test_machine(st,all_models);

        if(loss<args.threshold) {
            std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
            std::cerr<< st.get_state_matrix_string()<<std::endl;
            tm_data.push_back(std::pair<StateMatrix, double>(st, loss));
            if(loss==0){
                found_program=true;
                return tm_data;
            }
        } 
    }
    return tm_data;
}

std::vector<std::pair<StateMatrix, double>> Search::MonteCarloSearchMulticore() {
  
  // split work in partitions
  if (args.jobs > traversal_len) {
    args.jobs = traversal_len;
  }
  
  auto partition_len = traversal_len / args.jobs;
  auto partition_rest = traversal_len % args.jobs;

  // spawn  tasks asynchronously
  std::vector<std::future<std::vector<std::pair<StateMatrix, double>>>> works;
  for (auto i = 0u; i < args.jobs; ++i) {
    auto len = partition_len;
    if (i == args.jobs - 1) {
      len += partition_rest;
    }

    works.push_back(std::async([=]() {
        std::cerr << "Worker #" << i << " started @ partition ["  <<  len <<  "[" << std::endl;
        seed = (prime * seed) % 4079;
        std::cerr << "seed =>" << seed<< std::endl;
        auto o = MonteCarloSearch(len);
        std::cerr << "Worker #" << i << " finished" << std::endl;
        return o;
    }));
    
  }

  std::vector<std::pair<StateMatrix, double>> total;

  for (auto& f: works) {
    auto r = f.get();
    total.insert(end(total), begin(std::move(r)), end(std::move(r)));
  }

  return total; 

}

std::vector<std::pair<StateMatrix, double>> Search::SequentialSearchMulticore(){
  // split work in partitions

  if (args.jobs > traversal_len) {
    args.jobs = traversal_len;
  }
  
  auto partition_len = traversal_len / args.jobs;
  auto partition_rest = traversal_len % args.jobs;

  // spawn  tasks asynchronously
  std::vector<std::future<std::vector<std::pair<StateMatrix, double>>>> works;
  for (auto i = 0u; i < args.jobs; ++i) {
    auto offset = partition_len * i;
    auto len = partition_len;
    if (i == args.jobs - 1) {
      len += partition_rest;
    }

    works.push_back(std::async([=]() {
        std::cerr << "Worker #" << i << " started @ partition [" << offset << ", " << (offset + len) <<  "[" << std::endl;
        auto o = SequentialSearch(len, offset);
        std::cerr << "Worker #" << i << " finished" << std::endl;
      return o;
    }));

  }

  // await and merge results together

  std::vector<std::pair<StateMatrix, double>> total;

  for (auto& f: works) {
    auto r = f.get();
    total.insert(end(total), begin(std::move(r)), end(std::move(r)));
  }

  return total;
}

#include <cstring>
#include<filesystem>
#include <fstream>
#include <future>
#include <thread>
#include <string>
#include <sstream>
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
    std::unordered_map<std::string, double> results;
    if (std::strcmp(args.search_strategy, "Null") == 0){
        throw std::invalid_argument("search_strategy argument can not be null in this program");
    }
    else if (std::strcmp(args.search_strategy, "Sequential")== 0){
        results = SequentialSearchMulticore();

    }
    else if(std::strcmp(args.search_strategy, "Monte_Carlo") == 0){
       results = MonteCarloSearchMulticore();
    } 
    else if(std::strcmp(args.search_strategy, "Neural_Networks") == 0){
        std::cout << "ERROR ERROR" << std::endl;
        exit(-1); // fix latter
    }
    write_to_file(results);
}

std::vector<std::pair<std::string, double>> Search::SequentialSearch(TmId traversal_length, TmId traversal_offset){

    //stored data;
    std::vector<std::pair<std::string, double>> tm_data;
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
            tm_data.push_back(std::pair<std::string, double>(st.get_state_matrix_string(), loss));
            std::cerr<< st.get_state_matrix_string()<<std::endl;
            if(loss==0){
                found_program=true;
                return tm_data;
            }
        } 
        st.next();
        counter += 1;
    } while ((counter < traversal_length) && (found_program==false));
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

std::vector<std::pair<std::string, double>> Search::MonteCarloSearch(TmId traversal_length){
    std::vector<std::pair<std::string, double>> tm_data;
    StateMatrix st(args.states,args.alphabet_size);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);

    std::unordered_set<std::string> st_matrix;
    bool entry=false;

    
    std::minstd_rand rng{seed};
    
    if(seed==0){
      auto rd_dev = std::random_device{};
      std::seed_seq seq{rd_dev(), rd_dev(), rd_dev(), rd_dev()};
      std::mt19937 rng(seq);
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
            tm_data.push_back(std::pair<std::string, double>(st.get_state_matrix_string(), loss));
            if(loss==0){
                found_program=true;
                return tm_data;
            }
        } 
    }
    return tm_data;
}

std::unordered_map<std::string, double> Search::MonteCarloSearchMulticore() {
  
  // split work in partitions
  if (args.jobs > traversal_len) {
    args.jobs = traversal_len;
  }
  
  auto partition_len = traversal_len / args.jobs;
  auto partition_rest = traversal_len % args.jobs;

  // spawn  tasks asynchronously
  std::vector<std::future<std::vector<std::pair<std::string, double>>>> works;
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

  std::vector<std::pair<std::string, double>> total;

  for (auto& f: works) {
    auto r = f.get();
    total.insert(end(total), begin(std::move(r)), end(std::move(r)));
  }

  std::unordered_map<std::string, double> unordered_map;
    for(auto &pair:total){
        unordered_map.insert (pair);
    }
  return unordered_map;

}

std::unordered_map<std::string, double> Search::SequentialSearchMulticore(){
  // split work in partitions

  if (args.jobs > traversal_len) {
    args.jobs = traversal_len;
  }
  
  auto partition_len = traversal_len / args.jobs;
  auto partition_rest = traversal_len % args.jobs;

  // spawn  tasks asynchronously
  std::vector<std::future<std::vector<std::pair<std::string, double>>>> works;
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

  std::vector<std::pair<std::string, double>> total;

  for (auto& f: works) {
    auto r = f.get();
    total.insert(end(total), begin(std::move(r)), end(std::move(r)));
  }
    std::unordered_map<std::string, double> unordered_map;
    for(auto &pair:total){
        unordered_map.insert (pair);
    }
  return unordered_map;
}



void Search::write_to_file(std::unordered_map<std::string, double> results){
    if(results.empty()){return;}
    auto alphabet_subfolder=std::to_string(args.alphabet_size);
    auto state_subfolder=std::to_string(args.states);
    auto search_mode_subfolder=args.search_strategy;
    auto nc_subfolder = parse_target_file_to_get_nc(args.target_file);
    
    std::string path = "results/"+alphabet_subfolder+"/"+state_subfolder+"/"+search_mode_subfolder+"/"+nc_subfolder+"/";
    std::filesystem::create_directories(path);
    std::string file = path + args.target_file;
    std::ofstream outFile(file);
    for (auto &el: results){
        outFile  << el.first << "\t" << el.second << std::endl;
    }
}

std::string parse_target_file_to_get_nc(std::string &file_name){
  std::stringstream parse_file(file_name);
    std::vector<std::string> seglist;
    std::string segment;
    while(std::getline(parse_file, segment, '/'))
    {
      seglist.push_back(segment);
    }
    file_name=seglist.back();
    if(seglist.size()==5){
      return seglist[3];
    }
    return seglist.back();
}

#include <cstring>
#include<filesystem>
#include <fstream>
#include <future>
#include <queue>
#include <thread>
#include <string>
#include <sstream>
//#include <unordered_set>

#include "node.h"
#include "search.h"
#include "util.h"


Search::Search(Args args, double weight): args(args),loss(args,weight),traversal_len(args.traversal_len), seed(args.sd){
    TmId max_value = get_tm_cardinality(args.states, args.alphabet_size); 
    if (args.traversal_len==0 || args.traversal_len> max_value){
        traversal_len=max_value;
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
    else if(std::strcmp(args.search_strategy, "Tree_Search")==0){
       results = TreeSearchMulticore();
       //TreeSearch(traversal_len, 42,0);
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
            tm_data.push_back(std::pair<std::string, double>(st.get_state_matrix_string(), loss));
            if(loss==0){
                std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
                std::cerr<< st.get_state_matrix_string()<<std::endl;
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
            tm_data.push_back(std::pair<std::string, double>(st.get_state_matrix_string(), loss));
            if(loss==0){
              std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
              std::cerr<< st.get_state_matrix_string()<<std::endl;
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
        std::cerr << "Worker #" << i << " TMs to test :"  <<  len <<  "..." << std::endl;
        seed = (prime * seed) % 4079;
        //std::cerr << "seed =>" << seed<< std::endl;
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

bool ThreadSafeUnorderedSet::safe_insert(std::string str){

  // thread1 -> aaa
  // #2 -> aaa
  this->visitedNodesSync.lock();

  bool result = this->visitedNodes.insert(str).second;
  // true #1 _ false #2
  this->visitedNodesSync.unlock();
  // false #2 - #1 true
  return result;
}

bool ThreadSafeUnorderedSet::contains(std::string str){
  return this->visitedNodes.count(str)==1;
}

std::unordered_map<std::string, double> Search::TreeSearchMulticore(){
// split work in partitions
  
  if (args.jobs > traversal_len) {
    args.jobs = traversal_len;
  }

  TmId partition_len = div128by32(traversal_len, args.jobs);
  TmId partition_rest = traversal_len % args.jobs;

  std::vector<unsigned int> seedPerTheard;
  seedPerTheard.push_back((prime * seed) % 4079);

  // 
  for (auto i = 0u; i < args.jobs-1; ++i) {
    seedPerTheard.push_back((prime * seedPerTheard[i]) % 4079);
  }

  

  // spawn  tasks asynchronously
  std::vector<std::future<std::vector<std::pair<std::string, double>>>> works;
  for (auto i = 0u; i < args.jobs; ++i) {
    auto len = partition_len;
    if (i == args.jobs - 1) {
      len += partition_rest;
    }
    std::cerr<<"partition_len:"<<partition_len <<std::endl;

    works.push_back(std::async([=]() {
        std::cerr << "Worker #" << i << " TMs to test :"  <<  len <<  "..." << std::endl;
        
        auto o = TreeSearch(len, seedPerTheard[i], i);
        std::cerr << "Worker #" << i << " finished" << std::endl;
        return o;
    }));
    
  }

  std::vector<std::pair<std::string, double>> total;

  for (auto& f: works) {
    auto r = f.get();
    //total.insert(end(total), begin(std::move(r)), end(std::move(r)));
  }
  exit(-9);

  std::unordered_map<std::string, double> unordered_map;
    for(auto &pair:total){
        unordered_map.insert (pair);
    }
  return unordered_map;
}

std::vector<std::pair<std::string, double>> Search::TreeSearch(TmId traversal_length, unsigned int randSeed, unsigned int threadId){

  const unsigned int MAX_PATIENCE = 40u;
  unsigned int current_patience = 0u;

  AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
  std::vector<std::pair<std::string, double>> tm_data;
  //std::unordered_set<std::string> visitedNodes;
  std::priority_queue<RuleMatrixNode> nodesToOpen;

  //std::unordered_set<std::string> DEBUG_VECTOR;

  std::vector<TuringRecord> possibleRules = get_possible_rules(args.states,args.alphabet_size);
  unsigned int counter=0u;
  // Add the starting rule to the nodes to open
  double loss;
  const double RAND_MAX_MACHINES=1u;
  //const double RAND_MAX_MACHINES=1u;

  // multiple samples for better initialization
  std::cerr << "seed =>" << randSeed<< " Worker: " << threadId << "                   |" << std::endl;
  std::minstd_rand rng{randSeed};

  for (auto i=0u;i<RAND_MAX_MACHINES;i++){

    StateMatrix st(args.states, args.alphabet_size);
    st.set_random(rng);
    RuleMatrixNode startNode(st.get_state_matrix_string(), test_machine(st,all_models));
    
    nodesToOpen.push(startNode);
    visitedNodes.safe_insert(startNode.identifier);
    //std::cerr << "loss =>" << loss<< " Worker: " << threadId << " new? " << startNode.identifier <<"                   |" << std::endl;
  }
  unsigned int numberPossibleSucessors = ((args.states*args.alphabet_size*3)-1)*(args.states*args.alphabet_size);
  unsigned int minNumberSucessors = 1000;// get from args
  double last_loss = 9999999.9;
 // std::uniform_int_distribution<int>  distr(range_from, range_to);
  std::vector<StateMatrix> sucessors;
  auto i=0u;
  std::cerr<<"search :"<<traversal_length<<std::endl;
  while((!nodesToOpen.empty()) && (i<traversal_length) && (found_program==false)) { // || 
    auto currentNode = nodesToOpen.top();
    nodesToOpen.pop();
    visitedNodes.safe_insert(currentNode.identifier);
    
    if (last_loss==currentNode.cost && currentNode.cost>args.threshold){
      if (current_patience++>MAX_PATIENCE){
        StateMatrix st(args.states, args.alphabet_size);
        st.set_random(rng);
        RuleMatrixNode newGenNode(st.get_state_matrix_string(), test_machine(st,all_models));

        while (!visitedNodes.safe_insert(newGenNode.identifier)){
          st.set_random(rng);
          newGenNode = RuleMatrixNode(st.get_state_matrix_string(), test_machine(st,all_models));
        }

        nodesToOpen = std::priority_queue<RuleMatrixNode>();
        std::cerr << bold_on << red_on << "Run out of patience | worker: " << threadId << bold_off << std::endl;

        nodesToOpen.push(newGenNode);

        current_patience = 0u;
        continue;
      }
    }else{
      last_loss = currentNode.cost;
    }

    ++counter;

    StateMatrix st(currentNode.identifier, args.states, args.alphabet_size);
    
    if (numberPossibleSucessors<minNumberSucessors){
      sucessors = generate_sucessors(st,possibleRules);
    }else{
      sucessors = generate_random_sucessors(st,possibleRules, minNumberSucessors, currentNode.cost);
    }
    //std::cerr<< "How many? "<< sucessors.size()<< " loss " <<currentNode.cost <<std::endl;

    for(auto&sucessor: sucessors){  
      if(!visitedNodes.contains(sucessor.get_state_matrix_string()) ){
        
        loss = test_machine(sucessor,all_models);

        if(loss<args.threshold) {
          tm_data.push_back(std::pair<std::string, double>(sucessor.get_state_matrix_string(), loss));
          if(loss==0){
            std::cerr<< bold_on  << green_on <<"Found SOLUTION !!"<<bold_off<<std::endl;
            std::cerr<< bold_on  << green_on <<"Number of machines ran " << i+RAND_MAX_MACHINES <<" | Number of nodes open "<< counter<<bold_off<<std::endl;
            std::cerr<< sucessor.get_state_matrix_string()<<std::endl;
            found_program=true;
            return tm_data;
          }
        }

        if((i++ % 1000)==0){//std::cerr<<"counter: "<<counter<< " i : "<< i << " Worker: "<< threadId<<std::endl;
          //std::cerr<< "St. String :"<< currentNode.identifier <<"; Cost : " <<currentNode.cost<< " Worker: "<< threadId <<std::endl;
          std::cerr<<"Cost : " <<currentNode.cost<< " T machines " << i << " Worker: "<< threadId <<std::endl;
        }
        RuleMatrixNode newNode(sucessor.get_state_matrix_string(), loss);
        nodesToOpen.push(newNode);
      }
    }
  }
  std::cerr<<"counter: "<<counter<<std::endl;
  return tm_data;
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

unsigned __int128 div128by32(unsigned __int128 x, uint64_t y)
{
    return x/y;
}


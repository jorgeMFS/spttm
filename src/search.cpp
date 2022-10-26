
#include <cstring>
#include<filesystem>
#include <fstream>
#include <future>
#include <queue>
#include <thread>
#include <string>
#include <sstream>

#include "search.h"
#include "util.h"


Search::Search(Args args, double weight): args(args),loss(args,weight),traversal_len(args.traversal_len), seed(args.sd){
    TmId max_value = get_tm_cardinality(args.states, args.alphabet_size); 
    if (args.traversal_len==0 || args.traversal_len> max_value){
        traversal_len=max_value;
    }
    this->change = args.states>2;
    //init();
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
    TopKResults topKresult(20u);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
    
    StateMatrix st(args.states,args.alphabet_size);
    TmId counter = 0;
    if (traversal_offset > 0) {
        st.set_by_index(traversal_offset);
    }

    do {
        double loss = test_machine(st,all_models);
        
        topKresult.add(std::pair<double, std::string>(loss, st.get_state_matrix_string()));

        if(loss==0){
            std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
            std::cerr<< st.get_state_matrix_string()<<std::endl;
            found_program=true;
            return topKresult.to_vector();
        }
        
        st.next();
        counter += 1;
    } while ((counter < traversal_length) && (found_program==false));

    return topKresult.to_vector();
}

double Search::test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models) {
    
    TuringMachine tm(st);
    all_models.reset();
    
    for (auto i = 0u; i < args.tape_iterations ; ++i){
        TapeMoves tpMove = tm.act(); 
        all_models.update_tables(tpMove, tm.turingTape);
    }

    std::vector<MarkovTable> mkv_vector=all_models.get_markov_tables();
    return loss.compute_loss(mkv_vector);
}

std::pair<double, double> Search::test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models, unsigned int tapes_iter_short, RuleMatrixNode father_node) {

    if (this->change){

    
    TuringMachine tm(st);
    all_models.reset();
    for (auto i = 0u; i < tapes_iter_short ; ++i){
        TapeMoves tpMove = tm.act(); // grave esti antaŭe
        all_models.update_tables(tpMove, tm.turingTape);
    }

    std::vector<MarkovTable> mkv_vector=all_models.get_markov_tables();
    double son_short_loss = loss.compute_loss(mkv_vector);

    if (father_node.short_cost==son_short_loss){
      return std::pair<double, double> (father_node.cost * 1.01, son_short_loss);
    }

    // 
    for (auto i = tapes_iter_short; i < args.tape_iterations ; ++i){
        TapeMoves tpMove = tm.act(); 
        all_models.update_tables(tpMove, tm.turingTape);
    }

    mkv_vector=all_models.get_markov_tables();
    return std::pair<double, double> (loss.compute_loss(mkv_vector), son_short_loss);
    }else{
      
   TuringMachine tm(st);
    all_models.reset();

    for (auto i = 0u; i < args.tape_iterations ; ++i){
        TapeMoves tpMove = tm.act(); 
        all_models.update_tables(tpMove, tm.turingTape);
    }

    auto mkv_vector=all_models.get_markov_tables();
    auto main_loss = loss.compute_loss(mkv_vector);
    auto short_loss = main_loss;
    return std::pair<double, double> (main_loss, short_loss);
    }
}

std::pair<double, double> Search::test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models, unsigned int tapes_iter_short) {
    if (this->change){

  
    TuringMachine tm(st);
    all_models.reset();
    for (auto i = 0u; i < tapes_iter_short ; ++i){
        TapeMoves tpMove = tm.act(); // grave esti antaŭe
        all_models.update_tables(tpMove, tm.turingTape);
    }

    std::vector<MarkovTable> mkv_vector=all_models.get_markov_tables();
    double short_loss = loss.compute_loss(mkv_vector);

    
    for (auto i = tapes_iter_short; i < args.tape_iterations ; ++i){
        TapeMoves tpMove = tm.act(); 
        all_models.update_tables(tpMove, tm.turingTape);
    }

    mkv_vector=all_models.get_markov_tables();
    return std::pair<double, double> (loss.compute_loss(mkv_vector), short_loss);
      }else{
    
   TuringMachine tm(st);
    all_models.reset();

    for (auto i = 0u; i < args.tape_iterations ; ++i){
        TapeMoves tpMove = tm.act(); 
        all_models.update_tables(tpMove, tm.turingTape);
    }

    auto mkv_vector=all_models.get_markov_tables();
    auto main_loss = loss.compute_loss(mkv_vector);
    auto short_loss = main_loss;
    return std::pair<double, double> (main_loss, short_loss);
    
      }
}

std::vector<std::pair<std::string, double>> Search::MonteCarloSearch(TmId traversal_length, unsigned int randSeed){
    StateMatrix st(args.states,args.alphabet_size);
    AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);
    TopKResults topKresult(20u);
    std::cerr << "seed =>" << randSeed << std::endl;

    std::unordered_set<std::string> st_matrix;
    bool entry=false;

    std::minstd_rand rng{randSeed};

    for (auto counter = 0ull; counter < traversal_length; counter++) {
        if(found_program){
            return topKresult.to_vector();
        }

        do{
            st.set_random(rng);
            entry = st_matrix.insert(st.get_state_matrix_string()).second;
        }
        while(entry==false);
        
        
        double loss = test_machine(st,all_models);

        topKresult.add(std::pair<double, std::string>(loss, st.get_state_matrix_string()));
        if(loss==0){
          std::cerr<< bold_on  << green_on <<"Found Candidate, loss:" << bold_off <<bold_on << cyan_on<< loss << bold_off <<std::endl;
          std::cerr<< st.get_state_matrix_string()<<std::endl;
          found_program=true;
          return topKresult.to_vector();
        }
        
    }
    return topKresult.to_vector();
}

std::unordered_map<std::string, double> Search::MonteCarloSearchMulticore() {
  
  // split work in partitions
  if (args.jobs > traversal_len) {
    args.jobs = traversal_len;
  }
  auto partition_len = traversal_len / args.jobs;
  auto partition_rest = traversal_len % args.jobs;

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

    works.push_back(std::async([=]() {
        std::cerr << "Worker #" << i << " TMs to test :"  <<  len <<  "..." << std::endl;
        //std::cerr << "seed =>" << seed<< std::endl;
        auto o = MonteCarloSearch(len, seedPerTheard[i]);
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

TopKResults::TopKResults(unsigned int maxSize): maxSize(maxSize){}

void TopKResults::add(std::pair<double, std::string> result){
  if (this->topFoundResults.size()>=this->maxSize){
    this->topFoundResults.pop();
  }
  this->topFoundResults.push(result);
}

std::vector<std::pair<std::string, double>> TopKResults::to_vector(){

  std::vector<std::pair<std::string, double>> data;

  auto max_size = this->topFoundResults.size();
  for (auto i=0u; i<max_size;i++){
    auto r =this->topFoundResults.top();
    data.push_back(std::pair<std::string, double>(r.second, r.first));
    this->topFoundResults.pop();
  }

  return data;
}



bool ThreadSafeUnorderedSet::safe_insert(std::string str){

  // thread1 -> aaa
  // #2 -> aaa
  this->visitedNodesSync.lock();

  bool result = this->visitedNodesSet.insert(str).second;
  // true #1 _ false #2
  this->visitedNodesSync.unlock();
  // false #2 - #1 true
  return result;
}

bool ThreadSafeUnorderedSet::contains(std::string str){
  return this->visitedNodesSet.count(str)==1;
}


bool comparePair(std::pair<std::string, double> i1, std::pair<std::string, double>  i2)
{
    return (i1.second < i2.second);
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
        std::cerr << "Worker #" << i << " finished | vector size: " << o.size() << std::endl;
        return o;
    }));
    
  }

  std::vector<std::pair<std::string, double>> total;

  for (auto& f: works) {
    auto r = f.get();
    total.insert(end(total), begin(std::move(r)), end(std::move(r)));
  }
  //std::cerr << "| vector size: " << total.size() << std::endl;
  
  std::unordered_map<std::string, double> unordered_map;

    for(auto &pair:total){
        //std::cerr << i++<<" rules: " << pair.first << " " << pair.second << std::endl;
        unordered_map.insert (pair);
    }
  
  //std::cerr << "| unordered_map size: " << unordered_map.size() << std::endl;

  return unordered_map;
}

std::vector<std::pair<std::string, double>> Search::TreeSearch(TmId traversal_length, unsigned int randSeed, unsigned int threadId){

  const unsigned int MAX_PATIENCE = 40u;
  const unsigned int ITERATIONS_FOR_PREMATURE_SON_FATHER_CHECK = std::min(args.alphabet_size*args.states, 50lu);
  unsigned int current_patience = 0u;

  AllInteractiveMarkovModel<InteractiveMarkovModel> all_models(args.k, args.alphabet_size, args.alpha);

  //std::unordered_set<std::string> visitedNodes;
  std::priority_queue<RuleMatrixNode> nodesToOpen;
  TopKResults topKresult(20u);
  //std::unordered_set<std::string> DEBUG_VECTOR;

  std::vector<TuringRecord> possibleRules = get_possible_rules(args.states,args.alphabet_size);
  unsigned int counter=0u;
  // Add the starting rule to the nodes to open
  double loss;
  std::pair<double, double > loss_pair;
  const double RAND_MAX_MACHINES=10u;
  //const double RAND_MAX_MACHINES=1u;

  // multiple samples for better initialization
  std::cerr << "seed =>" << randSeed<< " Worker: " << threadId << "                   |" << std::endl;
  std::minstd_rand rng{randSeed};

  for (auto i=0u;i<RAND_MAX_MACHINES;i++){

    StateMatrix st(args.states, args.alphabet_size);
    st.set_random(rng);
    RuleMatrixNode startNode(st.get_state_matrix_string(), test_machine(st, all_models, ITERATIONS_FOR_PREMATURE_SON_FATHER_CHECK));

    while (visitedNodes.contains(startNode.identifier)){
      st.set_random(rng);
      startNode = RuleMatrixNode(st.get_state_matrix_string(), test_machine(st,all_models, ITERATIONS_FOR_PREMATURE_SON_FATHER_CHECK));
    }

    nodesToOpen.push(startNode);
    //visitedNodes.safe_insert(startNode.identifier);
    //std::cerr << "loss =>" << startNode.cost<< " Worker: " << threadId << " new? " << startNode.identifier <<"                   |" << std::endl;
  }
  //exit(-9);
  double last_loss = 9999999.9;
 // std::uniform_int_distribution<int>  distr(range_from, range_to);
  std::vector<StateMatrix> sucessors;
  auto i=0u;
  //std::cerr<<"search :"<<traversal_length<<std::endl;
  while((!nodesToOpen.empty()) && (i<traversal_length) && (found_program==false)) { // || 
    auto currentNode = nodesToOpen.top();
    nodesToOpen.pop();

    if (!visitedNodes.safe_insert(currentNode.identifier)){
      continue;
    }

    topKresult.add(std::pair<double, std::string>(currentNode.cost, currentNode.identifier));
    //std::cerr<< "Hip: topResults size "<< topKresult.topFoundResults.size() << std::endl;
    
    if (last_loss>=currentNode.cost){
      if (current_patience++>compute_max_patience(currentNode.cost, MAX_PATIENCE)){
        nodesToOpen = std::priority_queue<RuleMatrixNode>();
        //std::cerr << bold_on << red_on << "Run out of patience | worker: " << threadId << bold_off << std::endl;
        for (auto i=0u;i<RAND_MAX_MACHINES;i++){
          StateMatrix st(args.states, args.alphabet_size);
          st.set_random(rng);
          RuleMatrixNode newGenNode(st.get_state_matrix_string(), test_machine(st,all_models, ITERATIONS_FOR_PREMATURE_SON_FATHER_CHECK));

          while (visitedNodes.contains(newGenNode.identifier)){
            st.set_random(rng);
            newGenNode = RuleMatrixNode(st.get_state_matrix_string(), test_machine(st,all_models, ITERATIONS_FOR_PREMATURE_SON_FATHER_CHECK));
          }
          nodesToOpen.push(newGenNode);
        }

        current_patience = 0u;
        continue;
      }
    }else{
      current_patience = 0u;
    }

    last_loss = currentNode.cost;

    ++counter;

    StateMatrix st(currentNode.identifier, args.states, args.alphabet_size);
    

    //sucessors = generate_random_sucessors_w_mutations(st,possibleRules, currentNode.cost);
    sucessors = generate_random_sucessors(st,possibleRules, currentNode.cost, rng);
    
    //std::cerr<< "How many? "<< sucessors.size()<< " loss " <<currentNode.cost <<std::endl;

    for(auto&sucessor: sucessors){ 
      if (found_program) break;
      if( !visitedNodes.contains(sucessor.get_state_matrix_string()) ){
        
        loss_pair = test_machine(sucessor,all_models, ITERATIONS_FOR_PREMATURE_SON_FATHER_CHECK, currentNode);
        loss = loss_pair.first;

        if(loss<args.threshold) {
          
          if(loss==0){
            std::cerr<< bold_on  << green_on <<"Found SOLUTION !!"<<bold_off<<std::endl;
            std::cerr<< bold_on  << green_on <<"Number of machines ran " << i+RAND_MAX_MACHINES <<" | Number of nodes open "<< counter<<bold_off<<std::endl;
            std::cerr<< sucessor.get_state_matrix_string()<<std::endl;
            found_program=true;
            // converting a priority_queue to a vector to keep the same API
            // It would be more efficient to define iterators in the API
            topKresult.add(std::pair<double, std::string>(loss, sucessor.get_state_matrix_string()));
            //std::cerr<< "At exit time: "<< topKresult.topFoundResults.size() << std::endl;
            return topKresult.to_vector();
          }
        }
        i++;
        //if((i++ % 100000)==0){//std::cerr<<"counter: "<<counter<< " i : "<< i << " Worker: "<< threadId<<std::endl;
          //std::cerr<< "St. String :"<< currentNode.identifier <<"; Cost : " <<currentNode.cost<< " Worker: "<< threadId <<std::endl;
        //  std::cerr<<"Cost : " <<currentNode.cost<< " T machines " << i << " Worker: "<< threadId <<std::endl;
        //}
        RuleMatrixNode newNode(sucessor.get_state_matrix_string(), loss_pair);
        nodesToOpen.push(newNode);
      } 
    }
  }
  //std::cerr<<"counter: "<<counter<<std::endl;
  //std::cerr<< "At exit time: "<< topKresult.topFoundResults.size() << std::endl;
  return topKresult.to_vector();
}

void Search::write_to_file(std::unordered_map<std::string, double> results){

    std::vector<std::pair<std::string, double>> total;

    for (auto &el: results){
      total.push_back(el);
    } 

    std::sort(total.begin(), total.end(), comparePair);

    if(results.empty()){return;}

    auto alphabet_subfolder=std::to_string(args.alphabet_size);
    auto state_subfolder=std::to_string(args.states);
    auto search_mode_subfolder=args.search_strategy;
    auto nc_subfolder = parse_target_file_to_get_nc(args.target_file);
    
    std::string path = "results/"+alphabet_subfolder+"/"+state_subfolder+"/"+search_mode_subfolder+"/"+nc_subfolder+"/";
    std::filesystem::create_directories(path);
    std::string file = path + args.target_file;
    std::ofstream outFile(file);
    for (auto &el: total){
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


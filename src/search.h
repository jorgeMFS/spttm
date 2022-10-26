/**
    markovTable.h
    Purpose: Performes Sequential Search in a given domain of TM(sigma,state).
    
    Steps:
    A) Given a target string Y, it performs a sequential traversal search for the full TMs in the domain TM(sigma,state).
    B) Each TM runs n times. After running n iterations, the loss is determined for each iteration, by using KullbackLeiblerDivergence.
    C) When a match is found, or the threshold met, the machine stops, and outputs: (the statematrix, the divergence value, the tape output and run time.) 
    
    Optional, have time limit for running the machines. (i.e. parameter of runs or parameters for time)


    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once

#include "interactiveMarkovModel.h"
#include "parseArgs.h"
#include "loss.h"
#include "node.h"
#include <unordered_set>
#include <mutex>
#include <unordered_map>
#include <queue>

struct ThreadSafeUnorderedSet
{
  std::unordered_set<std::string> visitedNodesSet;
  std::mutex visitedNodesSync;

  bool safe_insert(std::string str);
  bool contains(std::string str);
};

struct TopKResults
{
  std::priority_queue<std::pair<double, std::string>> topFoundResults;
  unsigned int maxSize;

  TopKResults(unsigned int maxSize);
  std::vector<std::pair<std::string, double>> to_vector();
  void add(std::pair<double, std::string> result);

};

struct Search{
    
    

    Search(Args args, double weight);
    
    private:
        Args args;
        Loss loss;
        TmId traversal_len;
        unsigned int seed;
        ThreadSafeUnorderedSet visitedNodes;
        bool found_program=false;
        unsigned int file_counter=0;
        unsigned int prime = 65519;
        
        std::unordered_map<std::string, double> SequentialSearchMulticore();
        std::unordered_map<std::string, double> MonteCarloSearchMulticore();
        std::unordered_map<std::string, double> TreeSearchMulticore();
        
        std::vector<std::pair<std::string, double>> TreeSearch( TmId traversal_length, unsigned int randSeed, unsigned int threadId);

        std::vector<std::pair<std::string, double>> SequentialSearch(TmId traversal_length, TmId traversal_offset);
        std::vector<std::pair<std::string, double>> MonteCarloSearch(TmId traversal_length, unsigned int randSeed);
        double test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models);
        std::pair<double, double> test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models, unsigned int tapes_iter_short, RuleMatrixNode father_node);
        std::pair<double, double> test_machine(StateMatrix &st, AllInteractiveMarkovModel<InteractiveMarkovModel> &all_models, unsigned int tapes_iter_short);

        void write_to_file(std::unordered_map<std::string, double> results);

    public:
      void init();

};
unsigned __int128 div128by32(unsigned __int128 x, uint64_t y);
std::string parse_target_file_to_get_nc(std::string &file_name);
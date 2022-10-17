/**
    normalizedCompressionMarkovTable.h
    Purpose: Measure Statistical Complexity of Turing Machine Tapes.
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once

#include "metrics.h"
#include "markovTable.h"

struct NormalizedCompressionMarkovTable{
    
    MarkovTable mrkvTable;
    
    NormalizedCompressionMarkovTable(unsigned int k, unsigned int alphabet_size, double alpha);

    Metrics update(const Tape& tape);
    double normalization_base(unsigned int length_of_tape, unsigned int cardinality);
    double normalization_profile(unsigned int cardinality);
    int sum_all_elements_vector(std::vector<unsigned int>& subvector_markovtable);
    double calculateLog(int index_value, int sum_all_line_elem);
    void reset();
};


 template <typename T> 
 struct NormalizedCompressionMulipleContexts{
        std::vector<T> NormalizedCompressionMarkovTable;

        NormalizedCompressionMulipleContexts(const std::vector<unsigned int>& k, unsigned int alphabet_size, double alpha){
            for (auto k_el : k){
                NormalizedCompressionMarkovTable.emplace_back(k_el, alphabet_size, alpha);
            }
        }   

        std::vector<Metrics> update(const Tape& tape){
            std::vector<Metrics> metrics_vector;
            for(auto ncTable : NormalizedCompressionMarkovTable){
                metrics_vector.push_back(ncTable.update(tape));
            }
            return metrics_vector;
        }

        Metrics update_get_metrics_min_nc(const Tape& tape){
            std::vector<Metrics> metrics_vector;
            for(auto ncTable : NormalizedCompressionMarkovTable){
                metrics_vector.push_back(ncTable.update(tape));
            }
            return get_min_nc(metrics_vector);
        }
        Metrics update_get_metrics_avg_nc(const Tape& tape){
            std::vector<Metrics> metrics_vector;
            for(auto ncTable : NormalizedCompressionMarkovTable){
                metrics_vector.push_back(ncTable.update(tape));
            }
            return averageMetrics(metrics_vector);
        }

        Metrics update_get_metrics_max_nc(const Tape& tape){
            std::vector<Metrics> metrics_vector;
            for(auto ncTable : NormalizedCompressionMarkovTable){
                metrics_vector.push_back(ncTable.update(tape));
            }
            return get_max_nc(metrics_vector);
        }

        void reset(){
            for(auto i=0u;i<NormalizedCompressionMarkovTable.size();++i){
                NormalizedCompressionMarkovTable[i].reset();
            }
        }
 };
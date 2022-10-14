/**
    interactiveMarkovModel.h
    Purpose: Fill and Update Markov Tables Interactively.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/
#pragma once
#include <algorithm>
#include "markovTable.h"

/**
 * Used to interact with the tape and the MarkovTable Structs.
 */
struct InteractiveMarkovModel{
    MarkovTable mrkvTable;
    bool isFilled;

    /**
     * @param k. Size of context to consider on the MarkovTable.
     * @param alphabet_size. Size of the alphabet being considered by the tm.
     */
    InteractiveMarkovModel(unsigned int k, unsigned int alphabet_size, double alpha);
    // InteractiveMarkovModel(const InteractiveMarkovModel& other_model);
    /** Retrieve a copy of the inner Markov table */
    MarkovTable get_markov_table() const;
    double get_value(unsigned int i, unsigned int j) const;
    double get_value_from_normalized_fcm(unsigned int i, unsigned int j) const;
    /** reads Markov Table and Normalizes it line by line*/
    void normalize(double &lambda);
    
    /** Get normalized fcm */
    std::vector<double> get_normalized_vector() const;

    /** reads input vector and creates Markov Model*/
    void  fill_with_vector(const std::vector<unsigned int> &input_vector);

    /**sets markovTable of Interactive Markov Model */
    void set_markov_table(MarkovTable mkvtab);

    /** Retrieve a copy of the inner Markov table */
    unsigned int obtain_sum_occurrences_markov_table() const;

    /** Retrieve a copy of the inner Markov table vector */
    std::vector<unsigned int> get_markov_table_vector() const;
    /**
     * Updates Markov Table. It interacts directly with the table and controls its filling.
     * First if the tape has more that 10 elements filled by the turing machine it uses the fill function to read all those instances and fill the 
     * Markov Table. Afterwards at each iteration it uses the it_update_table function to update the table.
     *
     * @param previous_value . int of the previous value on the tape.
     * @param tape. Tape object of the Turing Machine.
     */
    void update_table(TapeMoves tpMv, Tape tape);
     /**
     * Uses previous table and updates it using new tape.
     * @param tape. Tape object of the Turing Machine.
     */
    void add_other_tape(const Tape& tape);
    private:
    /**
     * Fills Markov Table. It interacts directly with the table and controls its filling.
     * Reads tape which on the space which was written by the Turing machine and using that info fills the MarkovTable.
     * 
     * @param tape. Tape object of the Turing Machine.
     */
    void fill(const Tape& tape);
    /**
     *  Updates Markov Table at each iteration. It does it by adding at the table +1 in the context of the turing machine action on the table,
     *  and removing -1 in the context that existed previously in the tape. 
     *
     *  @param previous_value . int of the previous value on the tape.
     *  @param tape. Tape object of the Turing Machine.
     */
    void it_update_table(TapeMoves tpMv, Tape &tape);
    void it_edit_table(TapeMoves tpMv, size_t changedPosition, Tape &tape);
    void it_add_correcting_value_left(TapeMoves tpMv,Tape &tape);
    void it_add_correcting_value_right(TapeMoves tpMv,Tape &tape);
    void edit(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, TapeMoves tpMv, size_t changedPosition, Tape &tape);
};

template <typename T> 
struct AllInteractiveMarkovModel{ 
    std::vector<T> InteractiveMarkovModels;

    AllInteractiveMarkovModel(const std::vector<unsigned int>& k, unsigned int alphabet_size, double alpha){
        for (auto iterator = k.begin(); iterator != k.end(); ++iterator){
            InteractiveMarkovModels.emplace_back(*iterator, alphabet_size, alpha);
        }
    }   

    void set_markov_tables(MarkovTable mkvtab, unsigned int index){
        this->InteractiveMarkovModels[index].set_markov_table(mkvtab);
    }

    MarkovTable get_markov_tables(unsigned int index) const {
        return this->InteractiveMarkovModels[index].get_markov_table();
    }

    std::vector<double> get_value(unsigned int i, unsigned int j) const{
        std::vector<double> value_vec;
        for(auto k=0u;i<InteractiveMarkovModels.size();++k){ 
            value_vec.push_back(InteractiveMarkovModels[k].get_value(i,j));
        }
        return value_vec;
    }

    
    std::vector<double>  get_value_from_normalized_fcm(unsigned int i, unsigned int j) const{
        std::vector<double> value_vec;
        for(auto k=0u;i<InteractiveMarkovModels.size();++k){ 
            value_vec.push_back(InteractiveMarkovModels[k].get_value_from_normalized_fcm(i,j));
        }
        return value_vec;
    }

    std::vector<std::vector<double>> get_normalized_vector() const {
        std::vector<std::vector<double>> normalized_fcms;

        for(auto k=0u;k<InteractiveMarkovModels.size();++k){ 
            normalized_fcms.push_back(InteractiveMarkovModels[k].get_normalized_vector());
        }
        
        return normalized_fcms;
    }


    void normalize(double &lambda){
        for(auto i=0u;i<InteractiveMarkovModels.size();++i){
            InteractiveMarkovModels[i].normalize(lambda);
        }
    }

    std::vector<unsigned int> get_markov_table_vectors(unsigned int index) const{
        return this->InteractiveMarkovModels[index].get_markov_table_vector();
    }

    std::vector<MarkovTable> get_markov_tables() const{
       std::vector<MarkovTable>  markov_models;
        for(auto i=0u;i<InteractiveMarkovModels.size();++i){
             markov_models.push_back(InteractiveMarkovModels[i].get_markov_table());
        }
        return markov_models;
    }

    unsigned int obtain_sum_occurrences_markov_tables(unsigned int index) const{
        return this->InteractiveMarkovModels[index].obtain_sum_occurrences_markov_table();
    }

    void fills(unsigned int index, const Tape &tape){
        this->InteractiveMarkovModels[index].fill(tape);
    }
    void  fill_with_vector(const std::vector<unsigned int> &input_vector){
        for(auto i=0u;i<InteractiveMarkovModels.size();++i){
            InteractiveMarkovModels[i].fill_with_vector(input_vector);
        }

    }

    void fill_with_vector_from_tape(const std::vector<unsigned int> &input_vector, std::vector<unsigned int> &k){
        std::vector<unsigned int> vector_to_read;
         for(auto i=0u;i<InteractiveMarkovModels.size();++i){
            vector_to_read.clear();
            std::vector<unsigned int> vector_zeros(k[i], 0);
            vector_to_read.insert(vector_zeros.end(), input_vector.begin(), input_vector.end() );
            // delete later after debug
            for (auto i: vector_to_read){
                std::cout << i;
            }
            std::cout << std::endl;
            // 
            InteractiveMarkovModels[i].fill_with_vector(vector_to_read);
        }
    }

    void fills(const Tape &tape){
        for(auto i=0u;i<InteractiveMarkovModels.size();++i){
            InteractiveMarkovModels[i].fill(tape);
        }
    }

    void add_other_tapes(unsigned int index, const Tape& tape){
        this->InteractiveMarkovModels[index].add_other_tape(tape);    
    }

    void add_other_tapes(const Tape& tape){
        for(auto i=0u;i<InteractiveMarkovModels.size();++i){
         InteractiveMarkovModels[i].add_other_tape(tape);  
        }
    }

    void update_tables(TapeMoves tpMv, Tape tape){ 
        auto update = [ tpMv, tape ](InteractiveMarkovModel & n) { n.update_table(tpMv, tape); };
        std::for_each(std::begin(InteractiveMarkovModels), std::end(InteractiveMarkovModels),update);
    }
};


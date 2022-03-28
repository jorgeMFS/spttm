/**
    interactiveMarkovModel.h
    Purpose: Fill and Update Markov Tables Interactively.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/
#pragma once

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
    void update_table(TapeMoves tpMv, Tape &tape);
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

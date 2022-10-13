/**
    interactiveMarkovModel.cpp
    Purpose: Fill and Update Markov Tables Interactively.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/
#include "interactiveMarkovModel.h"

InteractiveMarkovModel::InteractiveMarkovModel(unsigned int k, unsigned int alphabet_size, double alpha):
mrkvTable(k, alphabet_size, alpha),isFilled(false) {}

// InteractiveMarkovModel::InteractiveMarkovModel(const InteractiveMarkovModel& other_model){
//     this->mrkvTable=other_model.mrkvTable;
//     this->isFilled=other_model.isFilled;
// }
void InteractiveMarkovModel::set_markov_table(MarkovTable mkvtab){
    this->mrkvTable=mkvtab;
}

MarkovTable InteractiveMarkovModel::get_markov_table() const {
    return this->mrkvTable;
}

double InteractiveMarkovModel::get_value(unsigned int i, unsigned int j) const{
    return this->mrkvTable.get_value(i,j);
}

double InteractiveMarkovModel::get_value_from_normalized_fcm(unsigned int i, unsigned int j) const{
    return this->mrkvTable.get_value_from_normalized_fcm(i, j);
}

void InteractiveMarkovModel::normalize(double &lambda) {
    this->mrkvTable.normalize(lambda);
}

std::vector<double> InteractiveMarkovModel::get_normalized_vector() const {
    return this->mrkvTable.get_normalized_vector();
}


std::vector<unsigned int> InteractiveMarkovModel::get_markov_table_vector() const{
    return this->mrkvTable.get_vector();
}

unsigned int InteractiveMarkovModel::obtain_sum_occurrences_markov_table() const{
    auto vector = this->get_markov_table_vector();
    return std::accumulate(vector.begin(), vector.end(), 0);
}

void InteractiveMarkovModel::fill(const Tape &tape){

    this->mrkvTable.reset(); // here is a reset to the mkvtable

    auto b = begin(tape.tape) + tape.ind_left + 1;    
    auto e = begin(tape.tape) + tape.ind_right - this->mrkvTable.k; 
    for (auto it = b; it != e; ++it) {
        this->mrkvTable.at(&*it) += 1;
    }
}
void InteractiveMarkovModel::fill_with_vector(const std::vector<unsigned int> &input_vector){

    this->mrkvTable.reset(); // here is a reset to the mkvtable
    auto b = begin(input_vector);    
    auto e = end(input_vector) - this->mrkvTable.k; 
    for (auto it = b; it != e; ++it) {
        this->mrkvTable.at(&*it) += 1;
    }
}


void InteractiveMarkovModel::add_other_tape(const Tape& tape){
    auto b = begin(tape.tape) + tape.ind_left + 1;    
    auto e = begin(tape.tape) + tape.ind_right - this->mrkvTable.k; 
    for (auto it = b; it != e; ++it) {
        this->mrkvTable.at(&*it) += 1;
    }
}

void InteractiveMarkovModel::update_table(TapeMoves tpMv, Tape tape){
 
     auto diff_indexes = tape.ind_right - tape.ind_left;
    if ((diff_indexes) <= 3) {
        this->isFilled=false;}
  
    if( (diff_indexes > 2) && (diff_indexes > (this->mrkvTable.k +2)  )  ){

        if (!this->isFilled ) {
            fill(tape);
            this->isFilled = true;
            //this->mrkvTable.print();
        }
        else{
            it_update_table(tpMv,tape);
        }
    }
}

void InteractiveMarkovModel::it_update_table(TapeMoves tpMv, Tape &tape) {
    size_t previousPosition = tpMv.previousPosition;

    if(tpMv.moveRight){
        it_add_correcting_value_right(tpMv, tape);
    }
    else if(tpMv.moveLeft){
        it_add_correcting_value_left(tpMv, tape);        
    }
        it_edit_table(tpMv, previousPosition, tape);
}

void InteractiveMarkovModel::it_add_correcting_value_right(TapeMoves tpMv,Tape &tape){
    tape.tape[tpMv.previousPosition] = tpMv.previousValue;
    auto elem = tape.tape.begin() +tape.getposition() - this->mrkvTable.k;      
    this->mrkvTable.at(&*elem)+=1;
    tape.tape[tpMv.previousPosition] = tpMv.writtenValue;
}

void InteractiveMarkovModel::it_add_correcting_value_left(TapeMoves tpMv,Tape &tape){
    auto elem = tape.tape.begin() + tape.getposition();      
    tape.tape[tpMv.previousPosition] = tpMv.previousValue;
    this->mrkvTable.at(&*elem)+=1;
    tape.tape[tpMv.previousPosition] = tpMv.writtenValue;
}

void InteractiveMarkovModel::it_edit_table(TapeMoves tpMv, size_t changedPosition, Tape &tape){
    auto begin = tape.tape.begin();
    auto end = tape.tape.begin();
    size_t difference_to_left_edge = changedPosition-tape.ind_left;
    size_t difference_to_right_edge =tape.ind_right - changedPosition;
    auto k = this->mrkvTable.k;

    if(     (difference_to_left_edge > k)    &&     (difference_to_right_edge > k )   ) {  
        begin += changedPosition - k;
        end  += changedPosition + 1;
    }
    else if(    (difference_to_left_edge <= k)    &&    (difference_to_right_edge > k )  ){
        begin += tape.ind_left + 1;
        end += changedPosition + 1; //present position
    }
    else if(    (difference_to_left_edge > k)    &&     (difference_to_right_edge <= k )   ){
        begin += changedPosition - k;
        end += tape.ind_right - k;
    }
    else if(    (difference_to_left_edge <= k)    &&     (difference_to_right_edge <= k )   ){
        begin += tape.ind_left + 1;
        end += tape.ind_right - k;
    }
    else{
        std::cerr << "Illegal state:" << std::endl
            << "\tdifference_to_left_edge = " << difference_to_left_edge << std::endl
            << "\tdifference_to_right_edge = " << difference_to_right_edge << std::endl
            << "\tk = " << k << std::endl;
        exit(-4);
    }
    edit(begin, end, tpMv, changedPosition, tape);
}

void InteractiveMarkovModel::edit(std::vector<unsigned int>::iterator begin, std::vector<unsigned int>::iterator end, TapeMoves tpMv, size_t changedPosition, Tape &tape){
    for (auto it = begin; it != end; ++it) {
        this->mrkvTable.at(&*it)+=1;
    }
    tape.tape[changedPosition] = tpMv.previousValue;
    for (auto it = begin; it != end; ++it) {
        this->mrkvTable.at(&*it)-=1;
    }         
    tape.tape[changedPosition] = tpMv.writtenValue;    
}
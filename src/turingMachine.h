/**
    turingMachine.h
    Purpose: Create and execute Turing Machines.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once
#include <tuple>
#include <random>
#include <vector>
#include <iostream>
#include <algorithm>
#include<unordered_map>

#include "tmId.h"

using std::uniform_int_distribution;
using Rng = std::minstd_rand;


using RecordId = unsigned int;
using State = unsigned int;
using Char = unsigned int;
using Move = unsigned int;

struct TapeMoves{
  size_t previousValue;
  size_t previousPosition;
  bool moveRight;
  bool moveLeft;
  unsigned int indexRule;
  unsigned int writtenValue;
};

struct TuringRecord{
  Char write;
  Move move;
  State state;

  bool operator<(const TuringRecord& tr) const;
  bool operator==(const TuringRecord& tr) const;
  
  bool next(unsigned int number_of_states, unsigned int alphabet_size);
  bool next_complementary(TuringRecord tr, unsigned int number_of_states, unsigned int alphabet_size);
  static TuringRecord by_index(RecordId id, unsigned int number_of_states, unsigned int alphabet_size);
  std::string tr_to_string();
};

std::ostream& operator<<( std::ostream& o, const TuringRecord& r);
TuringRecord set_random(Rng & rng, unsigned int number_of_states, unsigned int alphabet_size, TuringRecord tr_original);

struct StateMatrix{
  std::vector<TuringRecord> v;
  unsigned int nbStates;
  unsigned int alphSz;
  StateMatrix();
  StateMatrix(unsigned int number_of_states, unsigned int alphabet_size);
  StateMatrix(std::string string_state_matrix, unsigned int number_of_states, unsigned int alphabet_size);
  bool operator==(const StateMatrix &st) const;
  bool operator<(const StateMatrix &st1) const;
  bool operator>(const StateMatrix &st1) const;
  
  StateMatrix create_son(const StateMatrix &st1) const;
  double similarity(StateMatrix st2);
  
  unsigned int get_number_rules() const;
  unsigned int get_alphabet() const;
  unsigned int get_number_states() const;
  std::string get_state_matrix_string();
  TuringRecord get_element(unsigned int index) const;

  void set_by_index(TmId id);
  void set_rule(unsigned int index, TuringRecord& tr);

  TmId calculate_index() const;
  std::vector<TuringRecord> get_state_matrix() const;
  unsigned int get_state_matrix_size() const;
  std::vector<Char> get_state_matrix_alphabet() const;
  
  /// Reset the state matrix into a uniformly random position.
  template<typename R>
  void set_random(R& rng) {
    auto write_dist = uniform_int_distribution<Char>(0, this->alphSz - 1);
    auto state_dist = uniform_int_distribution<State>(0, this->nbStates - 1);
    auto move_dist = uniform_int_distribution<Move>{0, 2};
    for (auto& st: this->v) {
      st.write = write_dist(rng);
      st.move = move_dist(rng);
      st.state = state_dist(rng);
    }
  }

  TuringRecord& at(Char alph, State state);
  const TuringRecord& at(Char alph, State state) const;
  unsigned int get_index(Char alph, State state); // retrieve index of StateMatrix;
  bool next();
  void print() const;
  void print_st_matrix_vector() const;
  std::string print_st_matrix_vector();
  const TuringRecord* chr_line(Char c) const;

};

StateMatrix determine_last_state_Matrix(const unsigned int& number_of_states, const unsigned int& alphabet_size);
std::vector<StateMatrix> generate_sucessors(StateMatrix st);

struct compare_st{
    bool operator()(const StateMatrix & st1, const StateMatrix  & st2);
};

struct Tape {
  std::vector<Char> tape;
  size_t position;
  size_t max_size;
  size_t ind_left;
  size_t ind_right;

  Tape();
  unsigned int get_size() const;
  public:
    Char getvalue();
    size_t getposition() const;
    size_t get_index() const;
    std::vector<Char>get_tape_vector(unsigned int context_k);
    std::unordered_map<unsigned int, double> get_tape_frequency();

    TapeMoves setandmove(Move relativePos, Char value);
    void resetTape();
    void print() const;
    std::string print_written_tape(bool print_to_console) const;

  private:
    void reserve_right(size_t amount);
    void reserve_left(size_t amount);
};

std::ostream& operator<<( std::ostream& o, const Tape& t);

struct TuringMachine {
  State state;
  Tape turingTape;
  StateMatrix stMatrix;

  TuringMachine(unsigned int number_of_states, unsigned int alphabet_size);
  TuringMachine(const StateMatrix& ruleMatrix);
  void set_tm(StateMatrix& ruleMatrix);
  Tape get_tape() const;
  unsigned int get_tape_size() const;
  size_t get_index() const;

  void reset_tape_and_state();
  TapeMoves act(bool detectCycle=false);
  StateMatrix get_tm_state_matrix() const;
  unsigned int get_alphabet_size() const;
  unsigned int get_number_of_states() const;
  std::pair<TapeMoves,unsigned int> act_rule();
  bool test_full_alphabet_in_state_matrix(double percentage) const;
  std::string print_written_tape(bool print_to_console)const; 
  std::vector<Char> run_machine(unsigned int tape_it, unsigned int k);
};

struct TMCycleDetector{
  bool cycledetector(TuringMachine &tm, unsigned int iterations);
  bool detector(std::vector<unsigned int> &ruleTape,std::vector<unsigned int> &pastTape, unsigned int maxWindowSize);
  void printTapeRulesAndValues(std::vector<unsigned int> &ruleTape,std::vector<unsigned int> &pastTape);
};
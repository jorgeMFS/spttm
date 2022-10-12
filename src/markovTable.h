/**
    markovTable.h
    Purpose: Construct Markov Tables.
             Measure Statistical Complexity.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once
#include <vector>
#include <iostream>

#include "turingMachine.h"

using Char = unsigned int;

/**
 *  Table of repetitions a given contex has for a given alphabet.
 *  Example:
 *  k/context=2, alphabet size=2,  string = -010110011-
 *
 *  __|_0_|_1_
 *  00| 0 | 1
 *  01| 1 | 2
 *  10| 1 | 1
 *  11| 1 | 0
 *
 *  Note: All this is mapped into a vector of integers.
 */
struct MarkovTable{
  std::vector<unsigned int> markovVector;
  unsigned int alphSz;
  unsigned int k;
  double alpha;

  /**
   * @param k. Size of context to consider on the MarkovTable.
   * @param alphabet_size. Size of the alphabet being considered by the tm.
   */
  MarkovTable(unsigned int k, unsigned int alphabet_size, double alpha);
  
  std::vector<double> normalize(double lambda) const;

  /**
   * Accesses position on Markov Table, by providing a pointer to a position on array of TM characters.
   */
  unsigned int& at(const Char* characters);

  /**
   *  Provides line of of Markov Table, by providing a pointer to a position on array of unsigned ints.
   */
  std::vector <unsigned int> getLine(const Char* characters) const;

  /* 
  * DEBUG FUNCTION predict value, get probability and update model 
  */
  std::pair<double,double> debug_predict_and_update(const Char* characters);

  /* 
  * predict value, get probability and update model 
  */
  double predict_and_update(const Char* characters);
  
  /**
  * Computes the probability of char for a given context 
  **/
  double probability(const Char* characters);

  /**
  * computes the numerator at index 
  **/
  double numerator(const Char* characters);

  /**
  * Computes the denominator = sum of values +(sum_of alpha* cardinality)
  */
  double denominator(const Char* characters) const;

  /// Resets Markov Table.
  void reset();

  /// Prints Markov Table.
  void print() const;
  
  /// Prints Markov Table.
  void print_all() const;

  /// Returns context k used on table.
  unsigned int get_context() const;
  
  /// Returns context alpha used on table.
  double get_alpha() const;


  std::vector<unsigned int> get_vector() const;

  private:

  /**
   * Takes in pointer to char array and returns the context number on the Markov Table.
   *
   * @param const Char*. Pointer to char array.
   * @return size_t. Returns context number on the Markov Table.
   */
  size_t str_to_index(const Char* x) const;
};
void generate_words(std::vector<char> &alphabet,const size_t &word_length, std::vector<std::string>& results);


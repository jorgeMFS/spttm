#pragma once

#include <vector>
#include <map>
#include "parseArgs.h"
#include "markovTable.h"

struct empty_string_exception: public std::exception {
    using std::exception::what;
    virtual const char* what() {
        return "empty input received";
    }
};

/** StringProcess Struct

    StringProcess Struct. Struct that reads input and evaluate Compression Capability of Turing machine.
*/
struct StringProcess{
    std::vector<std::map<char, unsigned int>> maps;
    std::vector<std::vector<unsigned int>> transcribed_vectors;
    unsigned int m_cardinality;
    double normalizer;
    std::vector<char>alphabet;
    std::vector<char>original_input_vector;
    /** Fetch a string from stdin and process it into the constructed object.
     * May throw an exception if the constructor fails to retrieve data from
     * the input stream, or `empty_string_exception` if the input string is empty.
     */
    StringProcess(size_t alphabet_size);
    StringProcess(std::string input_file,size_t alphabet_size);
    StringProcess(StateMatrix input_rm, unsigned int tape_it, unsigned int k);

    /** Returns copy of input vector */    
    std::vector<char> get_input_vector() const;
    std::vector<std::vector<unsigned int>> get_all_transcribed_vectors() const;
    std::vector<unsigned int> get_transcribed_vector(unsigned int const & index) const;
    
    std::vector<MarkovTable> get_models(Args &args, std::vector<unsigned int> &confirmation_vector, bool print_bool) const;


    /** Sets new input vector and recalculates normalizer*/    
    void set_input_vector_and_normalize(std::vector<unsigned int>& new_input_vector);
    void print_map(std::map<char, unsigned int> & map) const;
    void transcribe_vectors();
    /** Retrieve the cardinality of the string that was inserted. */    
    unsigned int cardinality() const;

    /**
        Reads each element of the input vector and using the Markov table executes the folowing steps:
        - For each element of the input in the context k it gets the value in the MarkovTable (function markovTable.at) and adds +1;
        - Gets the line of the MarkovTable for the specified context (markovTable.getLine) and adds to the vector +1 in each element;
        - Using this it calculates the sum of all elements of the line (sum_all_elements_vector)
        - And with the input value at the table and the sum of all elements of the line it determines log2 (calculateLog)
        - Adds the value to the compression variable.

        @param std::vector<unsigned int>& input. The reference to an input vector of the input string.
        @param MarkovTable. Markov Table of computed values.
        @return void.
    */
    double readinput(MarkovTable& markovTable, unsigned int index) const;
    std::vector<double> readinput(MarkovTable& markovTable) const;
    std::vector<double> readinput_multimarkovmodel(std::vector<MarkovTable>& markovTableVector, unsigned int index) const;
    double get_average_nrc(std::vector<MarkovTable>& markovTableVector, unsigned int index) const;
    
    /**  Returns size_t of the maximum alphabet cardinality between the input inserted by the user (alphabet_size) and the input string.
     * @param alphabet_size the cardinality of the alphabet
    */
    unsigned int maximum_cardinality_verifier(size_t alphabet_size);
    void transform_to_vector();
    unsigned int sum_all_elements_vector(std::vector<unsigned int>& subvector_markovtable) const; //may remove latter
   





    /**
        Prints the compression variable of the Struct StringProcess       
        @return void.
    */
    void print() const;
    void print_transcribed_vector(unsigned int index_transcribed_vector) const;
    void print_maps()const;
    void printInputVector() const;
    
    std::map<char, unsigned int> get_alphabet_map(unsigned int index) const;
    std::string tape_to_original_alphabet(std::string const & tape, unsigned int const & index) const;
private:
    void getinputContent();
    void getinputContent(StateMatrix input_rm, unsigned int tape_it, unsigned int vector_k);
    void get_input_content_from_file(std::string input_file);
    /**
        Calculates the normalization_base by multiplying the number of elements in the input times the log_2 of the cardinality of the alphabet;
        
        @param std::vector<unsigned int>& subvector_markovtable. The reference to a sub vector of the input.
        @return void.
    */
    void normalization_base();

    /**
        Calculates the negated logarithm of the ratio between two ints and returns a double;
        
        @return The negation of the log2 of (index_value/sum_all_line_elem).
    */
    static double calculateLog(double index_value, double sum_all_line_elem);
};


/**
    read_input.h
    Purpose: Read files provided into vector (unsigned int or chars).
    
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once

#include <string>
#include <vector>

struct ReadInput{
    std::vector<char> vecOfchrs;
    ReadInput(std::string filename);
    void getFileContent(std::string fileName);
    void printData() const;
    std::vector<char> get_input_vector() const;
    std::vector<unsigned int> get_input_vector_as_unsigned_int() const;
};
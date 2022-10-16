#include <iostream>
#include <fstream>

#include "read_input.h"
#include "util.h"

ReadInput::ReadInput(std::string filename):vecOfchrs(){
        getFileContent(filename);
    }

void ReadInput::getFileContent(std::string fileName){
    std::vector<std::string> vecOfStrs;
    // Open the File
    std::ifstream in(fileName.c_str());
    // Check if object is valid
    
    if(!in){
        std::cerr << "Cannot Open the File : " << fileName <<std::endl;
        exit(-1);
    }
    std::string str;
    // Read the next line from File untill it reaches the end.
    while (std::getline(in, str)){
        // Line contains string of length > 0 then save it in vector
        if(str.size() > 0){
            for (auto s:str){
                this->vecOfchrs.push_back(s);
            }
            vecOfStrs.push_back(str);
        }
    }
    //Close The File
    in.close();
}

void ReadInput::printData() const {   
    std::cerr <<bold_on << green_on << "Data :" << bold_off << std::endl;
    for (auto elem:vecOfchrs){
        std::cerr<<bold_on << cyan_on << elem << bold_off << ' ';
    }
    std::cerr<< std::endl;
}

std::vector<char> ReadInput::get_input_vector() const{
    return vecOfchrs;
}

std::vector<unsigned int> ReadInput::get_input_vector_as_unsigned_int() const{
    std::vector<unsigned int> v(vecOfchrs.begin(), vecOfchrs.end());    
    for(auto& i : v)
        i -= '0'; // make digits to numbers
    return v;
}
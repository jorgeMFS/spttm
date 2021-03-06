#include <iostream>
#include <fstream>

#include "read_input.h"


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
    for (auto elem:vecOfchrs){
        std::cout<< elem;
    }
    std::cout<< std::endl;
}

std::vector<char> ReadInput::get_input_vector() const{
    return vecOfchrs;

}

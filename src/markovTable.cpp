/**
    markovTable.cpp
    Purpose: Construct Markov Tables.
             Measure Statistical Complexity.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <math.h>
#include <vector>

#include "markovTable.h"
#include "util.h"


MarkovTable::MarkovTable(unsigned int k, unsigned int alphabet_size, double alpha):
  markovVector(alphabet_size* ipow(alphabet_size,k),0), normalized_fcm(alphabet_size* ipow(alphabet_size,k),0) ,alphSz(alphabet_size), k(k),alpha(alpha){}

unsigned int MarkovTable::get_context() const{
    return this->k;
}

void MarkovTable::normalize(double lambda, double (*f)(unsigned int)) {
    normalized_fcm.clear();
    // for each line, normalize by max value + alpha;
    unsigned int counter = 0;
    std::vector<double> line;
    double denominator=0.0;
    for(auto&& x: this->markovVector){    
        line.push_back(f(x)+lambda);
        denominator+=(f(x)+lambda);

        if(++counter == this->alphSz){
            counter=0;
            std::transform(line.begin(), line.end(), line.begin(), [denominator](double &c){ return c/denominator; });
            normalized_fcm.insert(normalized_fcm.end(), line.begin(), line.end());
            denominator=0;
            line.clear();
        }
    }
};

void MarkovTable::normalize(double lambda) {
    normalized_fcm.clear();
    // for each line, normalize by max value + alpha;
    unsigned int counter = 0;
    std::vector<double> line;
    double denominator=0.0;
    for(auto&& x: this->markovVector){    
        line.push_back(x+lambda);
        denominator+=(x+lambda);

        if(++counter == this->alphSz){
            counter=0;
            std::transform(line.begin(), line.end(), line.begin(), [denominator](double &c){ return c/denominator; });
            normalized_fcm.insert(normalized_fcm.end(), line.begin(), line.end());
            denominator=0;
            line.clear();
        }
    }
};

void MarkovTable::quadratic_normalize(double lambda){
    
    normalized_fcm.clear();
    
    // for each line, normalize by max value + alpha;
    unsigned int counter = 0;
    std::vector<double> line;
    double denominator=0.0;

    for(auto&& x: this->markovVector){    
        line.push_back(x*x+lambda);
        denominator+=(x*x+lambda);

        if(++counter == this->alphSz){
            counter=0;
            std::transform(line.begin(), line.end(), line.begin(), [denominator](double &c){ return c/denominator; });
            normalized_fcm.insert(normalized_fcm.end(), line.begin(), line.end());
            denominator=0.0;
            line.clear();
        }
    }
};

std::vector<double> MarkovTable::get_normalized_vector() const {
    return this->normalized_fcm;
}

std::vector<unsigned int> MarkovTable::get_vector() const{
    return this->markovVector;
}

double MarkovTable::get_alpha() const{
    return this->alpha;
}

unsigned int& MarkovTable::at(const Char* characters){
    return this->markovVector.at(
        (   (str_to_index(characters) +1) * alphSz  )   -   (alphSz - characters[k])    ); 
}

std::vector<unsigned int> MarkovTable::getLine(const Char* characters) const{
    auto it_begin = this->markovVector.begin() + (  (   (str_to_index(characters) + 1) * this->alphSz   ) - this->alphSz    );
    auto it_end = this->markovVector.begin() + (    (str_to_index(characters) + 1) * this->alphSz   );
    std::vector <unsigned int> newVec(it_begin,it_end);
    return newVec;
}

/**
* predict value, get probability and update model 
**/
double MarkovTable::predict_and_update(const Char* characters){
    double prob = probability(characters);
    at(characters)+=1;
    return prob;
}

double MarkovTable::probability(const Char* characters){   
    double value =  numerator(characters)/denominator(characters);
    return (-log2(value));
    // return (- DEFAULT_LOG2_LUT.log2(value));
}

std::pair<double,double> MarkovTable::debug_predict_and_update(const Char* characters){
    double value =  numerator(characters)/denominator(characters);
    double log = -log2(value);
    at(characters)+=1;
    return std::pair<double,double>(value, log);
}

double MarkovTable::get_value(unsigned int i, unsigned int j) const{
    return markovVector.at((i*alphSz)+j);
}

double MarkovTable::get_value_from_normalized_fcm(unsigned int i, unsigned int j) const{
    return normalized_fcm.at((i*alphSz)+j);
}

/**
* computes the numerator at index 
**/
double MarkovTable::numerator (const Char* characters){
    auto x = at(characters);
    auto y = (1.0 * alpha); 
    // return at(characters) + (1 * alpha);
    return x+y;
}

/**
* Computes the denominator = sum(row) + (alpha* cardinality)
*/
double MarkovTable::denominator(const Char* characters) const{
    auto vector_line = getLine(characters);
    double sum_of_elems = 0; 
    for (auto& n : vector_line){
        sum_of_elems += n;
    }
    return sum_of_elems + (alphSz*alpha);
}



void MarkovTable::reset() {
    std::fill(this->markovVector.begin(), this->markovVector.end(), 0);
}

size_t MarkovTable::str_to_index(const Char* x)  const{
    size_t o = 0;
    for (auto i = 0u; i < this->k; ++i) { //-1
        o = (o * this->alphSz) + x[i];
    }
    return o;
}

void MarkovTable::print_all() const{
    unsigned int counter = 0;
    std::cout << std::endl;
    std::cout << bold_on << underlined_on << green_on <<"Markov Model" << bold_off << std::endl << "\t";

    std::vector<char> alphabet;
    std::cout << std::endl << bold_on  << green_on << "_______"  << bold_off;
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on << "_______"  << bold_off;
    }
    std::cout<<std::endl<<"\t";
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on  << letter  << bold_off<< "\t";
        alphabet.push_back(std::to_string(letter)[0]);
    }
    std::vector<std::string> results;
    generate_words(alphabet, this->k,results);
   
    std::cout << std::endl << bold_on  << green_on << "-------" << bold_off;
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on << "-------" << bold_off;
    }

    unsigned int cntr = 0;
    std::cout << std::endl << bold_on << green_on  << results[cntr] << " | " << bold_off << "\t";

    for(auto&& x: this->markovVector){      
        std::cout << bold_on << cyan_on << x << "\t" << bold_off;
        if(++counter == this->alphSz){
            if(++cntr<results.size()){
                std::cout << std::endl << bold_on << green_on << results[cntr] << " | "  << bold_off << "\t";
                counter = 0;
            }
        }
    }
    std::cout << std::endl;
}

void MarkovTable::print_normalization() const{
 unsigned int counter = 0;
    std::cout << std::endl;
    std::cout << bold_on << underlined_on << green_on <<"FCM Normalized" << bold_off;

    std::vector<char> alphabet;
    std::cout << std::endl << bold_on  << green_on << "_______"  << bold_off;
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on << "_______"  << bold_off;
    }
    std::cout<<std::endl<<"\t";
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on  << letter  << bold_off<< "\t";
        alphabet.push_back(std::to_string(letter)[0]);
    }
    std::vector<std::string> results;
    generate_words(alphabet, this->k,results);
   
    std::cout << std::endl << bold_on  << green_on << "-------" << bold_off;
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on << "-------" << bold_off;
    }
    unsigned int cntr = 0;
    std::cout << std::endl << bold_on << green_on  << results[cntr] << " | " << bold_off << "\t";
    
    for(auto&& x: this->normalized_fcm){    

            if (x>0) {
                std::cout << bold_on << red_on << x << "\t" << bold_off;
            }
            else{
                std::cout << bold_on << cyan_on << x << "\t" << bold_off;
            } 
            if(++counter == this->alphSz){
                if(++cntr<results.size()){
                    std::cout << std::endl << bold_on << green_on << results[cntr] << " | "  << bold_off << "\t";
                    counter = 0;
                }
            }
    }
    std::cout << std::endl;

}
unsigned int MarkovTable::sum_all_elem() const{
     auto sum_of_elems=0u;
     for (auto& n : markovVector)
        sum_of_elems += n;
    return sum_of_elems;
}

unsigned int MarkovTable::sum_all_elem(double (*f)(unsigned int)) const{
     auto sum_of_elems=0u;
     for (auto& n : markovVector)
        sum_of_elems += f(n);
    return sum_of_elems;
}   


void MarkovTable::print() const{
    unsigned int counter = 0;
    std::cout << std::endl;
    std::cout << bold_on << underlined_on << green_on <<"Markov Model" << bold_off;

    std::vector<char> alphabet;
    std::cout << std::endl << bold_on  << green_on << "_______"  << bold_off;
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on << "_______"  << bold_off;
    }
    std::cout<<std::endl<<"\t";
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on  << letter  << bold_off<< "\t";
        alphabet.push_back(std::to_string(letter)[0]);
    }
    std::vector<std::string> results;
    generate_words(alphabet, this->k,results);
   
    std::cout << std::endl << bold_on  << green_on << "-------" << bold_off;
    for(auto letter=0u; letter<this->alphSz; ++letter){
        std::cout << bold_on  << green_on << "-------" << bold_off;
    }
    unsigned int cntr = 0;
    std::cout << std::endl << bold_on << green_on  << results[cntr] << " | " << bold_off << "\t";
    
    // unsigned int begin =0;
    // unsigned int end = alphSz;
    
    for(auto&& x: this->markovVector){    
        // auto first = markovVector.begin() + begin;
        // auto last = markovVector.begin() + end;
        // unsigned int sum_of_elems=0;
        // std::vector<unsigned int> newVec(first, last);
        // for (auto& n : newVec)
        //     sum_of_elems += n;
        // if (sum_of_elems>0){
            if (x>0) {
                std::cout << bold_on << red_on << x << "\t" << bold_off;
            }
            else{
                std::cout << bold_on << cyan_on << x << "\t" << bold_off;
            } 
            if(++counter == this->alphSz){
                if(++cntr<results.size()){
                    std::cout << std::endl << bold_on << green_on << results[cntr] << " | "  << bold_off << "\t";
                    counter = 0;
                }
                // begin+=alphSz;
                // end+=alphSz;
            }
        //}
    }
    std::cout << std::endl;
}


void generate_words(std::vector<char> &alphabet,const size_t &word_length, std::vector<std::string>& results){
    std::vector<size_t> index(word_length, 0);
    
    for (;;){

        std::string word(index.size(), ' ');
        
        for (size_t i = 0; i < index.size(); ++i){
            word[i] = alphabet[index[i]];
        }
        results.emplace_back(word);

        for (int i = index.size() - 1; ; --i){
            if (i < 0){
                return;
            }

            ++index[i];
            if (index[i] == alphabet.size()){
                index[i] = 0;
            }
            else{
                break;
            }
        }
    }
}

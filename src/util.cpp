/**
    util.cpp
    Purpose: Utility functions for the program.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/
#include <vector>
#include <iostream>
#include <random>
#include "util.h"
#include <functional> 

Log2Lut<double, 1000000> DEFAULT_LOG2_LUT {};
Log2CardLut<double, 100000> DEFAULT_LOG2_CARD_LUT{};

#define MAX 100000 

std::ostream& bold_on(std::ostream& os){
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os){
    return os << "\e[0m";
}

std::ostream& underlined_on(std::ostream& os){
 return os << "\e[4m";
}

std::ostream& green_on(std::ostream& os){
 return os << "\e[32m";
}
std::ostream& cyan_on(std::ostream& os){
 return os << "\e[36m";
}

std::ostream& red_on(std::ostream& os){
    return os << "\e[31m";
}

unsigned int compute_max_patience(double current_loss, unsigned int default_patience){
  //auto t = static_cast<unsigned int>(1/(2*current_loss) + default_patience);
  //std::cerr << t << " loss " << current_loss << " default " << default_patience << std::endl;
  return static_cast<unsigned int>(1/(2*current_loss) + default_patience);
}

void set_output( const std::string& name, const double& value){
    std::cerr << bold_on << cyan_on <<name << ": " << bold_off;  
    std::cerr << bold_on << value << bold_off << std::endl;
}

void set_output_units(const std::string& name, const double& value, const std::string& units){
    std::cerr << bold_on << cyan_on << name << ": " << bold_off; 
    std::cerr << bold_on << value << " ";
    std::cerr << units << bold_off << std::endl;
}

void draw_line(){
    std::cout<<"--------------------------------"<<std::endl;
}
bool randomBool() {
    static auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
    return gen();
}

TmId get_tm_cardinality(unsigned int states, unsigned int alphabet_size) {
    if ((alphabet_size ==2 && states>10) || (alphabet_size==3 && states>7) || (alphabet_size==4 && states>5) || (alphabet_size >4 && states>2) ){
        std::cerr<<bold_on<<red_on<<"Value larger than 128 bits, considering transversal lenght the same as max uint128 "<<bold_off<<std::endl;
        return UINT128_MAX;
    }
    
    std::cerr<<states<<":"<<alphabet_size<<std::endl;
    TmId nstates = states;
    TmId nalphabet = alphabet_size;
    TmId record_cardinality = nstates * nalphabet * 3;

    std::cerr<<"base:"<< record_cardinality <<", exp:"<< nstates * nalphabet<<std::endl;
    return ipow(record_cardinality, nstates * nalphabet);
}

int multiply(int x, int res[], int res_size) { 
  int carry = 0; 
  for (int i = 0; i < res_size; i++) { 
      int prod = res[i] * x + carry; 
      res[i] = prod % 10; 
      carry = prod / 10; 
  } 
  
  while (carry) { 
      res[res_size] = carry % 10; 
      carry = carry / 10; 
      res_size++; 
  } 
return res_size; 
} 
  
// This function finds power of a number x 
void power(int x, int n){
  if(n == 0 ){  
      std::cout<<"1"; 
      return; 
  }  
  int res[MAX]; 
  int res_size = 0; 
  int temp = x; 
  
  while (temp != 0) { 
      res[res_size++] = temp % 10; 
      temp = temp / 10; 
  } 
    
  for (int i = 2; i <= n; i++) 
      res_size = multiply(x, res, res_size); 

  for (int i = res_size - 1; i >= 0; i--) 
      std::cout << res[i]; 
} 

/// Calculate the full cardinality of all possible Turing machine state matrices
void tm_fl_cardinality(size_t states, size_t alphabet_size){
    int nstates = states;
    int  nalphabet = alphabet_size;
    auto record_cardinality = nstates * nalphabet * 3;
    power(record_cardinality, nstates * nalphabet);
}


/// Create the list of the number of machines with the growth of the number of states.
void tm_growth_with_cardinality(size_t max_number_states){
    size_t min_number_states = 1;
    size_t alphabet_size=2;

    if (max_number_states< min_number_states){std::cerr<< "please insert number of states >= 1"<< std::endl; exit(0);}
     std::cout << "state" << "\t" <<"Number of Turing Machines"<< std::endl;

    for(auto state = min_number_states; state != max_number_states; ++state){
        std::cout << state << "\t"; 
        tm_fl_cardinality(state, alphabet_size);
        std::cout << std::endl;
    }
}


unsigned int convert_szt_to_ui( size_t value ){
    return static_cast<unsigned int>( value );
}

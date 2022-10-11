/**
    stringProcess.cpp
    Purpose: Read Input String and evaluate Compression Capability of Turing machine.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

//Includes
#include <string>
#include <valarray>
#include <cassert>
#include <numeric>

#include "stringProcess.h"
#include "turingMachine.h"
#include "read_input.h"
#include "util.h"

//------------------------------------
using std::valarray;

StringProcess::StringProcess(size_t alphabet_size)
: m_cardinality(0),
  original_input_vector()
{
    this->getinputContent();
    this->maximum_cardinality_verifier(alphabet_size);
    this->normalization_base();
}

StringProcess::StringProcess(std::string input_file,size_t alphabet_size)
: m_cardinality(0),
  original_input_vector()
{
    this->get_input_content_from_file(input_file);
    this->maximum_cardinality_verifier(alphabet_size);
    this->normalization_base();
}


StringProcess::StringProcess(StateMatrix input_rm, unsigned int tape_it, unsigned int k): m_cardinality(input_rm.alphSz), original_input_vector()
{
    this->getinputContent(input_rm,tape_it,k);
    this->normalization_base();
}

void StringProcess::getinputContent(StateMatrix input_rm, unsigned int tape_it, unsigned int context_k){
  TuringMachine tm(input_rm);
  auto tape = tm.run_machine(tape_it, context_k);
  for (auto t:tape){
    // auto s =static_cast<char>(t);
    std::string str = std::to_string(t);
    std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
    original_input_vector.push_back(cstr[0]);
  }

  auto copy = this->original_input_vector;
  std::sort(copy.begin(), copy.end());
  copy.erase(std::unique(copy.begin(), copy.end()), copy.end());
  this->alphabet = copy;
  this->m_cardinality = copy.size();
  this->transform_to_vector(); 
  this->transcribe_vectors();
}


std::vector<char> StringProcess::get_input_vector() const {
  return this->original_input_vector;
}

std::vector<std::vector<unsigned int>> StringProcess::get_all_transcribed_vectors() const {
  return this->transcribed_vectors;
}

std::vector<unsigned int> StringProcess::get_transcribed_vector(unsigned int const & index) const{
  return this->transcribed_vectors[index];
}


void StringProcess::set_input_vector_and_normalize(std::vector<unsigned int>& new_input_vector){
  this->transcribed_vectors[0] = new_input_vector;
  this->normalization_base();
}


unsigned int StringProcess::cardinality() const {
  return this->m_cardinality;
}

void StringProcess::normalization_base(){
  this->normalizer = this->transcribed_vectors[0].size() * DEFAULT_LOG2_CARD_LUT.log2(this->m_cardinality); // can be used log_2 table..
}


std::vector<double> StringProcess::readinput(MarkovTable& markovTable) const {
  std::vector<double> nrc_values;
  for (auto vec: this->transcribed_vectors){
    double value = 0;
    for(auto it = vec.begin(); it!= vec.end() - markovTable.k; ++it){
      value += markovTable.probability(&*it);
    }
    nrc_values.push_back(value/this->normalizer);
  }

  markovTable.reset();
  return nrc_values;
}

double StringProcess::readinput(MarkovTable& markovTable, unsigned int index) const {
  double value = 0;
  auto vec = this->transcribed_vectors[index];

  for(auto it = vec.begin(); it!= vec.end() - markovTable.k; ++it){
    value += markovTable.probability(&*it);
  }
  markovTable.reset();
  return (value/this->normalizer);
}

std::vector<double> StringProcess::readinput_multimarkovmodel(std::vector<MarkovTable>& markovTableVector, unsigned int index) const {
  std::vector<double> nrc_values;
  auto vec = this->transcribed_vectors[index];
  for (auto markovTable: markovTableVector){
    double value = 0;
    for(auto it = vec.begin(); it!= vec.end() - markovTable.k; ++it){
      value += markovTable.probability(&*it);
    }
    nrc_values.push_back(value/this->normalizer);
    markovTable.reset();
  }
  return nrc_values;

}
double StringProcess::get_average_nrc(std::vector<MarkovTable>& markovTableVector, unsigned int index) const {
  auto v = readinput_multimarkovmodel(markovTableVector,index); 
  return std::reduce(v.begin(), v.end()) / v.size();
}
double StringProcess::calculateLog(double index_value, double index_sum){
    auto value = index_value / index_sum;   
    return (- DEFAULT_LOG2_LUT.log2(value));
}




unsigned int StringProcess::maximum_cardinality_verifier(size_t alphabet_size){
  auto alphsz = convert_szt_to_ui(alphabet_size);
  if ( this->m_cardinality > alphsz ){
    printf ("Input string has elements larger (%u) that the alphabet size (%u) [0-%u] \n", this->m_cardinality, alphsz , alphsz - 1);
    printf ("Assuming cardinality of the input string (%u) --> [0-%u] \n",this->m_cardinality, this->m_cardinality - 1);
  }
  else if( this->m_cardinality < alphsz ){
    printf ("The alphabet size (%u) is larger than the input string cardinality (%u) that  [0-%u] \n", alphsz, this->m_cardinality , this->m_cardinality - 1);
    printf ("Assuming default cardinality (%u) --> [0-%u] \n",alphsz, alphsz - 1);
    this->m_cardinality = alphsz;
  }
  return this->m_cardinality;
}

unsigned int StringProcess::sum_all_elements_vector(std::vector<unsigned int>& subvector_markovtable) const
{
    int sum_of_elems = 0;

    for (auto& n : subvector_markovtable){
        sum_of_elems += n;
        }
    
    // std::cout << "sum_all_line_elem = " << sum_of_elems << std::endl ;
    return sum_of_elems;
}


void StringProcess::print() const{
  std::cerr << "Normalizer of the String = " << this->normalizer << std::endl;
}

void StringProcess::print_transcribed_vector(unsigned int index_transcribed_vector) const{
  assert(index_transcribed_vector<this->transcribed_vectors.size());
  auto retrieved_vector=this->transcribed_vectors[index_transcribed_vector];
  for (auto it = retrieved_vector.begin(); it != retrieved_vector.end(); ++it){
      std::cout << " " << *it;
    }
  std::cout << std::endl;
}


void StringProcess::printInputVector()const{
  for (auto it = this->original_input_vector.begin(); it != this->original_input_vector.end(); ++it)
                  std::cout << " " << *it;
                std::cout << std::endl;
}

void StringProcess::getinputContent(){
  std::string str;
  while (std::getline(std::cin, str)){ 
    if (!str.empty()) {
        for (auto s:str){
            this->original_input_vector.push_back(s);
        }
    }
  }
  if (this->original_input_vector.empty()) {
    std::cerr << "Input file/string not provided"<< std::endl; 
    throw empty_string_exception{};
  }

  auto copy = this->original_input_vector;
  std::sort(copy.begin(), copy.end());
  copy.erase(std::unique(copy.begin(), copy.end()), copy.end());
  this->alphabet = copy;
  this->m_cardinality = copy.size();
  this->transform_to_vector(); 
  this->transcribe_vectors();
}

void StringProcess::get_input_content_from_file(std::string input_file){
  
  ReadInput read_input(input_file);
  original_input_vector = read_input.get_input_vector();

  if (this->original_input_vector.empty()) {
    std::cerr << "Input file/string not provided"<< std::endl; 
    throw empty_string_exception{};
  }

  auto copy = this->original_input_vector;
  std::sort(copy.begin(), copy.end());
  copy.erase(std::unique(copy.begin(), copy.end()), copy.end());
  this->alphabet = copy;
  this->m_cardinality = copy.size();
  this->transform_to_vector(); 
  this->transcribe_vectors();
}



void StringProcess::transcribe_vectors(){
  for (auto &map:this->maps){
    std::vector<unsigned int> vec;
    for (auto &letter : this->original_input_vector){
       auto it = map.find(letter);
       vec.push_back(it->second);
    }
    this->transcribed_vectors.push_back(vec);
  }
}


void StringProcess::transform_to_vector(){
    std::vector<unsigned int> number(this->m_cardinality);
    std::iota(number.begin(), number.end(), 0);
    do {
        std::map<char, unsigned int> map;
        for (auto it =0u; it < number.size(); ++it ) {
         map.insert(std::pair<char,int>(this->alphabet[it],number[it]));
        }
    this->maps.push_back(map);
    } while (std::next_permutation(number.begin(), number.end()));
}


void StringProcess::print_maps() const{
  for (auto dic:  this->maps){
    print_map(dic);
    std::cout << "-------------" << std::endl;
  }
}

void StringProcess::print_map(std::map<char, unsigned int> & map) const{
  for(auto elem : map){
    std::cerr << elem.first << " " << elem.second << std::endl;
  }
}

std::map<char, unsigned int> StringProcess::get_alphabet_map(unsigned int index) const {
  return this->maps[index];
}



std::string StringProcess::tape_to_original_alphabet(std::string const & tape, unsigned int const &index) const {
    std::string mapped_string ="";
    std::map<char, unsigned int> initmap = get_alphabet_map(index);
    std::map<unsigned int, char> reversed;

    for (auto i = initmap.begin(); i != initmap.end(); ++i){
    reversed[i->second] = i->first;
    }
    for (auto it=tape.begin(); it!=tape.end(); ++it){
      auto search2 = reversed.find(*it - '0');
      if (search2 != reversed.end()){
        mapped_string +=search2->second;
      }
    }
    return mapped_string;
}

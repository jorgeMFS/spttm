/**
    turingMachine.cpp
    Purpose: Create and execute Turing Machines.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <cstddef>
#include <memory>
#include <sstream>
#include <cassert>
#include <functional> 
#include "util.h"
#include "turingMachine.h"
#include <nonstd/span.hpp>


using nonstd::span;

bool TuringRecord::next(unsigned int number_of_states, unsigned int alphabet_size) {
  if (write < alphabet_size - 1) {
    write += 1;
    return true;
  }
  write = 0;
  if (move < 2) {
    move += 1;
    return true;
  }

  move = 0;
  if (state < number_of_states - 1) {
    state += 1;
    return true;
  }

  state = 0;
  return false;
}

bool TuringRecord::next_complementary(TuringRecord tr, unsigned int number_of_states, unsigned int alphabet_size){
  auto a=next(number_of_states, alphabet_size);
  if((tr.move==this->move && tr.state==this->state && tr.write==this->write) && !(0==this->move && 0==this->state && 0==this->write)){
    return next(number_of_states, alphabet_size);
  }
  return a;
}


std::ostream& operator<<( std::ostream& o, const TuringRecord& r) {
  o << "(" << r.write << "," << r.move << "," << r.state << ")";
  return o;
}

TuringRecord TuringRecord::by_index(RecordId id, unsigned int number_of_states, unsigned int alphabet_size) {
  // order of significance (least to most): character_write -> tape_move -> state
  RecordId nstates = number_of_states;
  RecordId nalphabet = alphabet_size;
  auto rest = id;
  auto write = static_cast<Char>(rest % nalphabet);
  rest /= nalphabet;
  auto move = static_cast<Move>(rest % 3);
  rest /= 3;
  auto state = static_cast<State>(rest % nstates);
  return TuringRecord { write, move, state };
}
std::string TuringRecord::tr_to_string(){
  return std::to_string(this->write) + "-" + std::to_string(this->move) + "-" + std::to_string(this->state) + "-";
}

bool TuringRecord::operator==(const TuringRecord& tr) const {
    return ((this->move == tr.move) && (this->state==tr.state) && (this->write == tr.write));
}


bool TuringRecord::operator<(const TuringRecord& tr) const {
    return (this->move + this->state + this->write) < (tr.move + tr.state + tr.write);
}

//-----------------------------------------------------------------------------
TuringRecord set_random(Rng &rng,unsigned int number_of_states, unsigned int alphabet_size, TuringRecord tr_original) {
  
  auto write_dist = uniform_int_distribution<Char>(0, alphabet_size - 1);
  auto state_dist = uniform_int_distribution<State>(0, number_of_states - 1);
  auto move_dist = uniform_int_distribution<Move>{0, 2};
  TuringRecord tr = tr_original;
  do{
    tr.write = write_dist(rng);
    tr.move = move_dist(rng);
    tr.state = state_dist(rng);}
  while(tr == tr_original);

  return tr;
}

std::vector<TuringRecord> get_possible_rules(unsigned int number_of_states, unsigned int alphabet_size){
  TuringRecord tr{0,0,0};
  std::vector<TuringRecord> tr_vec;
  tr_vec.push_back(tr);
  while(tr.next(number_of_states, alphabet_size)){
    tr_vec.push_back(tr);
  }
  return tr_vec;
}

void StateMatrix::set_by_index(TmId id) {
  auto rest = id;
  auto record_cardinality = this->nbStates * this->alphSz * 3;
  for (auto& st: this->v) {
    auto state_id = rest % record_cardinality;
    st = TuringRecord::by_index(state_id, this->nbStates, this->alphSz);
    rest /= record_cardinality;
    if (rest == 0) break;
  }
}

//-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////STATE MATRIX////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StateMatrix::StateMatrix():
  nbStates(0), alphSz(0){}

StateMatrix::StateMatrix(unsigned int number_of_states, unsigned int alphabet_size):
  v(alphabet_size * number_of_states, TuringRecord{0, 0, 0}), nbStates(number_of_states), alphSz(alphabet_size){}

StateMatrix::StateMatrix(unsigned int number_of_states, unsigned int alphabet_size, std::minstd_rand &rng):
  v(alphabet_size * number_of_states, TuringRecord{0, 0, 0}), nbStates(number_of_states), alphSz(alphabet_size){
    set_random(rng);
    }

StateMatrix::StateMatrix(std::string string_state_matrix, unsigned int number_of_states, unsigned int alphabet_size){
  
  std::vector<unsigned int> vec;

  std::stringstream ss(string_state_matrix);
  std::string str;
  for (int i; ss >> i;) {
      str += std::to_string(i);
      vec.push_back(i);    
      if (ss.peek() == '-')
          ss.ignore();
  }

  if(!(is_number(str) && (vec.size()%(3*number_of_states*alphabet_size)==0) )){
    fprintf (stderr,"Invalid input string of TM State Matrix : %s\n ", string_state_matrix.c_str());
    exit(0);
  }

  int NumSubstrings = vec.size() / 3;
  std::vector<TuringRecord> ret;

  for (auto i = 0; i < NumSubstrings ; i++){
    std::vector<int>   sub(&vec[i*3],&vec[(i*3) + 3]);
    unsigned int w = sub[0];
    unsigned int m = sub[1];
    unsigned int s = sub[2];

    std::string value = std::to_string(w) + "," + std::to_string(m) + "," + std::to_string(s);
    bool test = (w < alphabet_size) && (m<3) && (s<number_of_states);
    if (!test){
        std::cerr <<  bold_on<< red_on << "w :" << w << ", alphabet size :" << alphabet_size <<std::endl;
        std::cerr <<  bold_on<< red_on << "m :" << m << ", total moves :" << 3 <<std::endl;
        std::cerr <<  bold_on<< red_on << "s :" << s << ", Number of States :" << number_of_states <<std::endl;
        std::cerr << bold_on<< cyan_on<< "Alphabet Size" << bold_off << bold_on << ":" << alphabet_size << " , " << bold_off;
        std::cerr << bold_on<< cyan_on<< "Number of States" << bold_off << bold_on << ":" <<  number_of_states << bold_off << std::endl;
        fprintf (stderr,"Invalid input string of TM State Matrix in tuple : ");
        std::cerr << "(w,m,s) = " << value <<std::endl;
        exit(0);
    }
    //std::cout << w << m << s << std::endl;
    ret.push_back(TuringRecord{w,m,s});
  }
  this->v = ret;
  this->alphSz = alphabet_size;
  this->nbStates = number_of_states;
}

bool StateMatrix::operator==(const StateMatrix &st) const{
  if (v.size()==st.v.size()){
    for (auto a=0u;a!=v.size();++a){
      if (v[a]==st.v[a]){
        return false;
      }
    }
    return true;
  }
  else{
    return false;
  }
}

 bool compare_st::operator()(const StateMatrix & st1, const StateMatrix  & st2){
  if (st1.v.size()==st2.v.size()){
    for (auto a=0u;a!=st1.v.size();++a){
      if (st1.v[a]==st2.v[a]){
        return false;
      }
    }
    return true;
  }
  else{
    return false;
  }

}

bool StateMatrix::operator>(const StateMatrix &st1) const{
  auto in=0u;
  auto out=0u;
  for (auto a=0u;a!=st1.v.size();++a){
    if(st1.v[a]<v[a]){++in;}
    else{++out;}
  } 
  return in>out;

}

bool StateMatrix::operator<(const StateMatrix &st1) const{return this->v.size() < st1.v.size();}


std::vector<TuringRecord> StateMatrix::get_state_matrix() const{
  return this->v;
}

unsigned int StateMatrix::get_state_matrix_size() const{
  return this->v.size();
}


unsigned int StateMatrix::get_number_rules() const{
  return this->v.size();
}

unsigned int StateMatrix::get_number_states() const{
  return this->nbStates;
}

unsigned int StateMatrix::get_alphabet() const{
  return this->alphSz;
}

StateMatrix StateMatrix::create_son(const StateMatrix &stf) const{
  StateMatrix son(nbStates,alphSz);
  for (unsigned int a=0; a!=get_state_matrix_size(); ++a){
    auto b = randomBool();
    // auto b = rand() > (RAND_MAX / 2);
    if(b==1){
        son.v[a]=v[a];
    }
    else{
        son.v[a]=stf.v[a];
    }
  }
  return son;
}

std::vector<Char> StateMatrix::get_state_matrix_alphabet() const{
  std::vector<Char> writeVector;
    for (auto const& value: this->get_state_matrix()){
      if (std::find(writeVector.begin(), writeVector.end(), value.write) == writeVector.end()) {
        writeVector.push_back(value.write);
      }
  }
return writeVector;
};

TuringRecord StateMatrix::get_element(unsigned int index) const{
  if (index > this->v.size() - 1){
    fprintf (stderr,"Invalid index for TM State Matrix : %u\n ", index);
    exit(1);
  }
  return v[index];
}


std::string StateMatrix::get_state_matrix_string(){
  std::string state_matrix_string = "";
  for (auto tr :this->v){
    state_matrix_string+=tr.tr_to_string();
  }
  return state_matrix_string;
}

double StateMatrix::similarity(StateMatrix st2){
    double total_rules=static_cast<double>(get_number_rules());
    double in_common_rules=0;
    for(auto i = 0u; i != total_rules; ++i){
      if(this->v[i] == st2.v[i] ){
          ++in_common_rules;
      }
    }
    return in_common_rules/total_rules;
}

TmId StateMatrix::calculate_index() const {
  auto record_cardinality = this->nbStates * this->alphSz * 3;
  TmId id(0);
  for (auto it = rbegin(this->v); it != rend(this->v); ++it) {
    auto& st = *it;
    TmId st_index = st.write + (st.move + st.state * 3) * this->alphSz;
    id = id * record_cardinality + st_index;
  }
  return id;
}

TuringRecord& StateMatrix::at(Char alph, State state){
  return this->v.at(   (   (alph + 1) * this->nbStates   )   -   (   this->nbStates - state  )   );
}


const TuringRecord& StateMatrix::at(Char alph, State state) const {
  return this->v.at(   (   (alph + 1) * this->nbStates   )   -   (   this->nbStates - state  )   );
}

// retrieve index of StateMatrix;
unsigned int StateMatrix::get_index(Char alph, State state){
  return   (   (alph + 1) * this->nbStates   )   -   (   this->nbStates - state  )   ;
}

bool StateMatrix::next(){
  for(auto& e: v) {
    if (e.next(nbStates, alphSz)) {
      return true;
    }
  }
  return false;
}



void StateMatrix::set_rule(unsigned int index, TuringRecord& tr ) {
  if (index > this->v.size() - 1){
    fprintf (stderr,"Invalid index for TM State Matrix : %u\n ", index);
    exit(1);
  }
 this->v[index] = tr;
}
void StateMatrix::print() const{
    std::cout << std::endl << bold_on << green_on << underlined_on<< "Turing Machine State Matrix" << bold_off << std::endl<<"     ";

    std::cout << std::endl;
 
    std::cout << "   ";

    for (auto jj=0u;jj< nbStates ; jj++){
      std::cout << bold_on << green_on <<"    "<< jj << "     "<< bold_off;
    }
  
    std::cout << std::endl<< "   ";
     for (auto jj=0u;jj< nbStates ; jj++){
      std::cout<<bold_on << green_on <<"----------"<<bold_off;
    }
    std::cout << std::endl;

    for (auto ii=0u;ii< alphSz ; ii++){
        auto line = chr_line(ii);
        std::cout <<bold_on << green_on <<" " << ii << " |"<<bold_off;

        for (auto jj=0u;jj< nbStates ; jj++){
          auto& r = line[jj];
          std::cout << bold_on << cyan_on << r << " | "<<bold_off;
        }
        std::cout << std::endl;
    }
      
    std::cout << std::endl << "Lines --> Alphabet Letter "<< std::endl << "Columns --> TM State "<< std::endl;
    std::cout << "(w,m,s) --> (write,move,state)" << std::endl;

}

void StateMatrix::print_st_matrix_vector() const{
  for (auto ii=0u;ii< this->alphSz ; ii++){
    auto line = chr_line(ii);
    for (auto jj=0u;jj< nbStates ; jj++){
      auto& r = line[jj];
      std::cout << r << ";";
    }
  }
  std::cout << std::endl;
}
std::string StateMatrix::print_st_matrix_vector(){
std::string s = "";
for (auto ii=0u;ii< this->alphSz ; ii++){
    auto line = chr_line(ii);
    for (auto jj=0u;jj< nbStates ; jj++){
      auto& r = line[jj];
      unsigned int write = r.write;
      unsigned int move = r.move;
      unsigned int state = r.state;
      s += "(" + std::to_string(write) + ","  + std::to_string(move) + "," + std::to_string(state) + ")" + ";";
    }
  }
  return s;
}


const TuringRecord* StateMatrix::chr_line(Char c) const {
  return this->v.data() + c * this->nbStates;
}

StateMatrix determine_last_state_Matrix(const unsigned int& number_of_states, const unsigned int& alphabet_size){
  TuringRecord tr{alphabet_size-1,2,number_of_states-1};
  StateMatrix st_matrix(number_of_states,alphabet_size);
  for (long unsigned int i =0; i<st_matrix.v.size(); ++i ){
    st_matrix.v[i]=tr;
  }
  return st_matrix;
}

std::vector<StateMatrix> generate_sucessors(StateMatrix &st, std::vector<TuringRecord> &possible_rules){
  std::vector<StateMatrix> successors;

  unsigned int st_size=st.get_state_matrix_size();
  for(auto it=0u; it <st_size; ++it){
    auto current_record = st.get_element(it);
    for(auto j=0u; j <possible_rules.size(); ++j) {
      StateMatrix new_st(st);
      auto cmp_record=possible_rules[j];
      if(!(current_record==cmp_record)){
        // swap
        new_st.set_rule(it, cmp_record);
        successors.push_back(new_st);
      } 
    }  
  }

  assert(successors.size() == ((st_size*3)-1)*st_size);

  return successors;
}

std::vector<StateMatrix> generate_random_sucessors(StateMatrix &st, std::vector<TuringRecord> &possible_rules, double loss_value, std::minstd_rand rnd_gen){
  auto successors = generate_sucessors(st, possible_rules);

  unsigned int st_size=st.get_state_matrix_size();

  // magic stuff
  auto nMac = (((st_size*3)-1)*st_size);
  auto nMin = log2(nMac+1)*100;

  unsigned int number_outputs = static_cast<unsigned int>(2/loss_value + nMin);

  number_outputs = std::min(nMac, number_outputs);
  
  std::vector<StateMatrix> output_random_sucessors;
  
  // this injures the reproducibility
  std::sample(successors.begin(), successors.end(), std::back_inserter(output_random_sucessors), number_outputs, rnd_gen);

  return output_random_sucessors;
}

std::vector<StateMatrix> generate_random_sucessors_w_mutations(StateMatrix &st, std::vector<TuringRecord> &possible_rules, double loss_value, std::minstd_rand rnd_gen){

  auto successors = generate_random_sucessors(st, possible_rules, loss_value, rnd_gen);

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> real_dis(0.0, 1.0);
  std::uniform_int_distribution<> int_dis(0, possible_rules.size()-1);

  for (auto &successor: successors){
    
//    if (loss_to_probability(loss_value)>real_dis(gen)){
      // mutate
      for (auto i=0u;i<successor.v.size();i++){
        if (loss_to_probability(loss_value)>real_dis(gen)){ 
          successor.set_rule(i, possible_rules[int_dis(gen)]);
        }
      }
  //  }
  }

  return successors;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////TAPE///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Tape::Tape():
  tape(256, 0), position(128), max_size(256),ind_left(position-1),ind_right(position+1),ind_more_left(128){}

Char Tape::getvalue() {
  return this->tape[this->position];
}

size_t Tape::getposition() const {
  return this->position;
}

size_t Tape::get_index() const {
  return this->position-this->ind_more_left;
}

/**
    Given a relative position and a value this function:
    - writes new value on the tape on the position.
    - changes the position on the tape;
    
    
    It also:
    - Allocates space to the left or the right of the tape if the max_size is exceeded;
    - defines if index positions of the left and right of the tape have changed.

    @param relativePos . Relative position that the tm must move in the tape.
    @param value. Value that the Tm must write on the Tape.
    @return TapeMoves. Structure TapeMoves(previousValue,moveRight,moveLeft)
*/
TapeMoves Tape::setandmove(Move relativePos, Char value) {
 
  //previousValue; moveRight; moveLeft; indexRule; writtenValue;
  TapeMoves tapeMove{0,0,false,false,0,value};
  tapeMove.previousValue = this->tape[this->position];
  this->tape[this->position] = value;
  int replace_pos = relativePos - 1;
  tapeMove.previousPosition = this->position;
  this->position += replace_pos;
  
  if(this->position<this->ind_more_left){
    this->ind_more_left=this->position;
  }


  if (this->position >= this->ind_right){
      this->ind_right = this->position+1;
      tapeMove.moveRight = true;
  }
      
  else if (this->position <= this->ind_left){
      this->ind_left = this->position-1;
      tapeMove.moveLeft=true;
  }

  if (this->position >= this->max_size) {
    this->reserve_right(this->max_size);

  } 
  else if (this->position <= 10) {
    tapeMove.previousPosition +=this->max_size;
    this->reserve_left(this->max_size);

  }
  return tapeMove;
}

void Tape::resetTape() {
  std::fill(this->tape.begin(), this->tape.end(), 0);
  this->position = this->tape.size() / 2;
  this->ind_left = this->position-1;
  this->ind_right = this->position+1;
  this->ind_more_left = this->position;
}

void Tape::reserve_right(size_t amount) {
  this->max_size += amount;
  this->tape.resize(this->max_size);
}

void Tape::reserve_left(size_t amount) {
  this->max_size += amount;
  std::vector<Char> tape2 = this->tape;
  std::fill(this->tape.begin(), this->tape.end(), 0);
  this->tape.resize(amount);
  this->tape.insert(tape.end(), tape2.begin(), tape2.end());
  this->position = this->position + amount;
  this->ind_right = this->ind_right + amount;
  this->ind_left = this->ind_left + amount;
}

/* prints written part of the tape.*/


unsigned int Tape::get_size() const{
  return this->ind_right - this->ind_left;
}


void Tape::print() const{
  for (auto i = this->tape.begin() + this->ind_left - 8 + 1; i != this->tape.end(); ++i)
    std::cout << *i << ' ';
  std::cout<< std::endl;
} 

std::vector<Char>Tape::get_tape_vector(unsigned int context_k){
  auto b = ind_left - context_k  + 1 ; // To have k context at the begining    
  auto e = ind_right - context_k;
  std::vector<Char> tape_vector;
  for (auto it = b; it != e; ++it) {
    tape_vector.push_back(tape[it]);
  }
  return tape_vector;
}

std::unordered_map<unsigned int, double> Tape::get_tape_frequency(){
  std::unordered_map<unsigned int, double> d;
  auto b = ind_left; // To have k context at the begining    
  auto e = ind_right;
  auto diff = ind_right - ind_left;
  for (auto it = b; it != e; ++it) {
    d[tape[it]]++;
  }
  for (auto & x : d){
    x.second /= diff;
   }
  return d;
}


std::string Tape::print_written_tape(bool print_to_console) const{
   if(print_to_console){
     std::cerr << bold_on << underlined_on << green_on << "Written Tape" << bold_off << std::endl << std::endl;
     }
  std::string written_tape= "";
  for (auto j = this->tape.begin() + this->ind_left + 1; j != this->tape.begin() + this->ind_right; ++j){    
      if(print_to_console){
        std::cerr << bold_on << cyan_on <<*j << ' ' << bold_off;
        }
      written_tape += std::to_string(*j) + " ";
  }
  if(print_to_console){
    std::cerr << std::endl;
  }
  return written_tape;
}


std::ostream& operator<<( std::ostream& o, const Tape& t) {
  auto start = std::find_if(t.tape.begin(), t.tape.end(), [](auto c) { return c != 0; });
  auto end = std::find_if(t.tape.rbegin(), t.tape.rend(), [](auto c) { return c != 0; }).base() + 1;
  if (end <= start) {
    end = start + 1;
  }
  for (auto it = start; it != end; ++it) {
    std::cout << *it;
  }
  return o;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////TURING MACHINE//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TuringMachine::TuringMachine(unsigned int number_of_states, unsigned int alphabet_size):
  state(0), turingTape(), stMatrix(number_of_states,alphabet_size){}

TuringMachine::TuringMachine(const StateMatrix& ruleMatrix):
  state(0), turingTape(), stMatrix(ruleMatrix){}

void TuringMachine::set_tm(StateMatrix& ruleMatrix){
  stMatrix = ruleMatrix;
  reset_tape_and_state();
}

Tape TuringMachine::get_tape() const{
  return this->turingTape;
}
unsigned int TuringMachine::get_tape_size() const{
  return this->turingTape.get_size();
}

TapeMoves TuringMachine::act(bool detectCycle){
  TapeMoves tpMove;
  Char alphValue = turingTape.getvalue();
  auto tr = stMatrix.at(alphValue, this->state);
  
  tpMove = turingTape.setandmove(tr.move, tr.write);

  if (detectCycle) {
    auto ruleIndex = stMatrix.get_index(alphValue, this->state);
    tpMove.indexRule = ruleIndex;
  }
  this->state = tr.state;
  return tpMove;
}

std::tuple<TapeMoves,unsigned int,size_t> TuringMachine::act_rule(){
  TapeMoves tpMove;
  Char alphValue = turingTape.getvalue();
  auto tr = stMatrix.at(alphValue, this->state);
  tpMove = turingTape.setandmove(tr.move, tr.write);
  auto ruleIndex = stMatrix.get_index(alphValue, this->state);
  this->state = tr.state;

  return {tpMove,ruleIndex,turingTape.get_index()};
}

std::vector<Char> TuringMachine::run_machine(unsigned int tape_it, unsigned int k){
  for (auto i = 0u; i < tape_it; ++i){
    act(); // grave esti antaŭe
  }
  Tape tp = get_tape();
  auto obtained_tape = tp.get_tape_vector(k);
  reset_tape_and_state();
  return obtained_tape;
}


StateMatrix TuringMachine::get_tm_state_matrix() const{
  return this->stMatrix;
}

//TMCycleDetector::TMCycleDetector(){}
void TuringMachine::reset_tape_and_state(){
 this->turingTape.resetTape();
 this->state=0;
}

unsigned int TuringMachine::get_alphabet_size() const {
  return this->stMatrix.alphSz;
}

unsigned int TuringMachine::get_number_of_states() const {
  return this->stMatrix.nbStates;
}

std::string TuringMachine::print_written_tape(bool print_to_console)const {
  return this->turingTape.print_written_tape(print_to_console);
}

bool TuringMachine::test_full_alphabet_in_state_matrix(double percentage) const{
  std::vector<int> intersection; 
  std::vector<int> difference; 
  double similarity;
  
  auto alphabet_state_matrix = this->stMatrix.get_state_matrix_alphabet();
  std::sort(alphabet_state_matrix.begin(), alphabet_state_matrix.end());
  std::vector<unsigned int> alphabet_vector(this->get_alphabet_size());
  std::iota(alphabet_vector.begin(), alphabet_vector.end(), 0);
  /*
  std::cout << "\n alphabet_state_matrix elements:\n"; 
  for (auto st = alphabet_state_matrix.begin(); st != alphabet_state_matrix.end(); ++st) 
        std::cout << *st << ", "; 
  std::cout << '\n'; 
  std::cout << "\n alphabet_vector elements:\n"; 
  for (auto st = alphabet_vector.begin(); st != alphabet_vector.end(); ++st) 
      std::cout << *st << ", "; 
  std::cout << '\n'; 
   */
  
  if (alphabet_state_matrix==alphabet_vector){
    similarity=1;
  }
  else {
    std::set_intersection(alphabet_state_matrix.begin(), alphabet_state_matrix.end(),
                          alphabet_vector.begin(), alphabet_vector.end(),
                          std::back_inserter(intersection));


    std::set_difference(alphabet_state_matrix.begin(), alphabet_state_matrix.end(),
                          alphabet_vector.begin(), alphabet_vector.end(),
                          std::back_inserter(difference));
                          
    similarity = intersection.size()/(difference.size() + intersection.size());
  }
  /*if (similarity>=percentage){
    std::cout << "\nCommon elements:\n"; 
    for (auto st = intersection.begin(); st != intersection.end(); ++st) 
        std::cout << *st << ", "; 
    std::cout << '\n'; 

    std::cout << "\n different elements:\n"; 
    for (auto st = difference.begin(); st != difference.end(); ++st) 
        std::cout << *st << ", "; 
    std::cout << '\n'; 

  } */
  return  similarity>=percentage;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////TMCycleDetector////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool TMCycleDetector::cycledetector(TuringMachine &tm, unsigned int iterations){
  std::vector<unsigned int> appliedRules;
  std::vector<unsigned int> pastTape;
  unsigned int iter = 0;
  bool detected = false;
  while((iter < iterations) ){ //&& !detected
    TapeMoves tpMv = tm.act(true);
    appliedRules.push_back(tpMv.indexRule);
    pastTape.push_back(tpMv.writtenValue);
    ++iter;
  }  
  detected = detector(appliedRules,pastTape, 16);
  tm.reset_tape_and_state();
  // if (!detected){
  //   std::cout << "not detected"<<std::endl;
  //   printTapeRulesAndValues(appliedRules, pastTape);
  //   exit(42);
  // }
  return detected;
}

bool TMCycleDetector::detector(std::vector<unsigned int> &ruleTape, std::vector<unsigned int> &pastTape, unsigned int maxWindowSize){
  //std::cout<< "Rule tape size -->" << ruleTape.size() << std::endl;
  if (maxWindowSize > ruleTape.size()/3){
    // std::cerr<< "maxWindowSize changed From:" << maxWindowSize <<" --> " << ruleTape.size()/3 << std::endl;
    maxWindowSize = ruleTape.size()/3;
  }
  for (unsigned int windowSize=1; windowSize != maxWindowSize; ++windowSize){
    //std::cout<< "Window size -->" << windowSize + 1<< std::endl;
    for(unsigned int index = windowSize + 1; index < ruleTape.size() - 2*windowSize - 1; ++index){
      auto ruleFirst1 = &ruleTape[index];
      auto ruleLast1 = ruleFirst1 + windowSize + 1;
      auto ruleFirst2 = ruleLast1;
      auto ruleLast2 = ruleFirst2 + windowSize + 1;
      
      const span<unsigned int> ruleVector1{ruleFirst1, ruleLast1};
      const span<unsigned int> ruleVector2{ruleFirst2, ruleLast2};

      if (ruleVector1 == ruleVector2){
        //Second Test
        auto tapeFirst1= &pastTape[index];
        auto tapeLast1 = tapeFirst1 + windowSize + 1;
        auto tapeFirst2 = tapeLast1;
        auto tapeLast2 = tapeFirst2 + windowSize + 1;
        const span<unsigned int> TapeVector1{tapeFirst1, tapeLast1};
        const span<unsigned int> TapeVector2{tapeFirst2, tapeLast2};
        
        if (TapeVector1 == TapeVector2){
          // Validation 
          const span<unsigned int> pastCycle{tapeFirst1-(windowSize+1),tapeFirst1};
          const span<unsigned int> pastRules{ruleFirst1-(windowSize+1),ruleFirst1};
          if (TapeVector1 == pastCycle && ruleVector1 == pastRules){
            // std::cout << "DETECTED" << std::endl;
            return true;
          }
          
        }
      }
      
    }     
  }
  return false;
}

void TMCycleDetector::printTapeRulesAndValues(std::vector<unsigned int> &ruleTape, std::vector<unsigned int> &pastTape){
  std::cout << std::endl << "Rule Tape: ";
  for (auto i = ruleTape.begin(); i != ruleTape.end(); ++i)
      std::cout << *i << ' ';
  std::cout << std::endl << "Tape Elem: ";
  for (auto i = pastTape.begin(); i != pastTape.end(); ++i)
      std::cout << *i << ' ';
  std::cout << std::endl<<std::endl;
}


/*Run the machine 1 iteration,
 I give the tape, the input and the state as input and with the rules

    |   0  |   1 
  ----------------
  0 |   A  |   B
  ----------------
  1 |   C  |   D 

  ABCD ABCD
  ---- ----

*/

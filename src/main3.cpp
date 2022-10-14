// Goal generate_complex_tapes
// given s,a e it, compute complex tapes with threshold.
// Prints rules

#include "parseArgs.h"
#include "tprl.h"

int main (int argc, char **argv){
  Args argument = parseArgs(argc,argv);  
  printArgs(argument);
  bool print_all;
  char answer;
  std::cerr << " Do you want to print all tm information (Y/N)?\n";
  std::cin >> answer;
  switch( answer )
  {
  case 'Y': case 'y': 
    print_all = true;
    break;

  case 'N': case 'n':
    print_all = false;
    break;
  }

  tprl tprl(argument,print_all);
}
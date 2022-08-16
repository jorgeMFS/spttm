// Goal generate_complex_tapes
// given s,a e it, compute complex tapes with threshold.
// Prints rules

#include "parseArgs.h"
#include "tprl.h"

int main (int argc, char **argv){
  Args argument = parseArgs(argc,argv);  
  printArgs(argument);
  tprl tprl(argument);
  // 
  //
}
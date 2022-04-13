// Goal generate_complex_tapes
// given s,a e it, compute complex tapes with threshold.
#include "parseArgs.h"
#include "sptm.h"

int main (int argc, char **argv){
  Args argument = parseArgs(argc,argv);  
  printArgs(argument);
}
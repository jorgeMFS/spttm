/**
    main.cpp
    Purpose: Run Program

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <iomanip>

#include <sstream>
#include <numeric>
#include <cmath>

#include "parseArgs.h"
#include "util.h"
#include "sptm.h"

/*
  * It will iterate through all the lines in file and
  * put them in given vector
*/

int main (int argc, char **argv){

  Args argument = parseArgs(argc,argv);
  sptm sptm(argument);
  sptm.update();
  
}

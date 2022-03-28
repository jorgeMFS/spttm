/**
    sptm.h
    Purpose: Get inputs parameters, output metrics.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#include <vector>
#include "parseArgs.h"
#include "stringProcess.h"

// receives files
// updates TM
// outputs NRC
// waits to receive more files
// recalculates NCR

struct sptm{
    
    StringProcess str;
    unsigned int index;
    unsigned int input_amplitude;
    sptm(Args args);
    void update(); 
    std::tuple<StateMatrix, unsigned long long, unsigned int, double, unsigned int>read_file();
    double get_diff_amplitudes(unsigned int & tape_length);

};
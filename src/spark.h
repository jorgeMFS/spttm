/**
    spark.h
    Purpose: Search Tool for Turing Machines
    
    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/
#include <vector>

#include "parseArgs.h"
#include "stringProcess.h"

// don't forget amplitude...

struct spark{
    
    StringProcess str;
    unsigned int index;
    unsigned int num_it;
    unsigned int input_amplitude;
    public:
        spark(Args args);
        void update(); 
    private:
        void init(Args args);
        double get_diff_amplitudes(unsigned int & tape_length);

};

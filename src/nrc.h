#include "parseArgs.h"
#include "stringProcess.h"



struct nrc{

    StringProcess str;
    nrc(Args args);
    
    private:
        void init(Args args);

};
#include "parseArgs.h"
#include "stringProcess.h"


struct crss_ent{

    StringProcess str;
    crss_ent(Args args);
    
    private:
        void init(Args args);

};
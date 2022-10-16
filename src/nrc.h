/**
    nrc.h
    Purpose: Computes NRC.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once

#include "parseArgs.h"
#include "stringProcess.h"



struct nrc{

    StringProcess str;
    nrc(Args args);
    
    private:
        void init(Args args);

};
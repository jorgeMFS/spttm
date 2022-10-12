

#include "parseArgs.h"
#include "nrc.h"

int main (int argc, char **argv){

    Args argument = parseArgs(argc,argv);
    printArgs(argument);
    nrc nrc(argument);
}

#!python
#!/usr/bin/env python

import os
import sys
from os import listdir, path, makedirs
from os.path import isfile, join, getsize
from pprint import pprint
from collections import Counter
import itertools
import time
from config import SEED,TAPE_ITERATIONS,FILE_PREFIX,NUMBER_GEN_FILES

#Paths
working_dir="../"


def _initialize():
    os.chdir(working_dir)
    current_directory = os.getcwd()
    print("Current working directory:", current_directory)
    
    for alphabet in range(2,5):
        for state in range(2,31):
            SEARCH_MODE='M'
            os.system(f'./tprl   -e {SEED} -s {state} -a {alphabet} -i {TAPE_ITERATIONS}  -f {FILE_PREFIX}  -N {NUMBER_GEN_FILES}')            
            SEARCH_MODE='S'
            os.system(f'./tprl   -e {SEED} -s {state} -a {alphabet} -i {TAPE_ITERATIONS}  -f {FILE_PREFIX}  -N {NUMBER_GEN_FILES}')

def main():
    _initialize()


if __name__ == "__main__":
    if "/" in sys.argv[0]:
        print("ERROR: Please run this script inside of src_python_tests/! There are relative paths defined in this code that need to be respected!")
    else:
        main()
        pprint("Files generated successfully")
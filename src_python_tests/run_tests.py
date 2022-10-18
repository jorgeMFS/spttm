
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
from config import TAPE_ITERATIONS,K_INIT,K_LAST,ALPHA,LAMBDA,THRESHOLD,THREADS,MACHINES_TO_RUN

#Paths
working_dir="../"

#Variable
SEARCH_MODE=["S","M"]

def _initialize():
    os.chdir(working_dir)
    current_directory = os.getcwd()
    print("Current working directory:", current_directory)
    
    for alphabet in range(2,5):
        for state in range(2,31):
            for nc in range(0,1,10):
                path = "tapeTestPool/"+str(alphabet)+"/"+str(state)+"/"+str(nc)+"/"; 
                if os.path.exists(path):
                    for filename in os.listdir(path):
                        f = os.path.join(path, filename)
                        for searchMode in SEARCH_MODE:
                            os.system(f'./spark   -s {state} -a {alphabet} -A {ALPHA} -i {TAPE_ITERATIONS} -l {LAMBDA} -k {K_INIT}:{K_LAST} -T {THRESHOLD} -S {searchMode} -t {f} -j {THREADS} -n {MACHINES_TO_RUN}')
def main():
    _initialize()


if __name__ == "__main__":
    if "/" in sys.argv[0]:
        print("ERROR: Please run this script inside of src_python_tests/! There are relative paths defined in this code that need to be respected!")
    else:
        main()
        pprint("Test completed successfully")
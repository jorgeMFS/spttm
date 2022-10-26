
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
from config import SEED_C,TAPE_ITERATIONS,FILE_PREFIX,NUMBER_GEN_FILES,NUMBER_TMS_TEST_FILE_CREATION,ALPHA

#Paths
working_dir="../"


def _initialize():
    os.chdir(working_dir)
    
    for alphabet in range(2,5,2):
        for state in [ 2**x for x in range(1,7)]:
            os.system(f'./tprl   -e {SEED_C} -s {state} -a {alphabet} -A {ALPHA} -i {TAPE_ITERATIONS} -k 2 -f {FILE_PREFIX}  -N {NUMBER_GEN_FILES} -n {NUMBER_TMS_TEST_FILE_CREATION}') 

def main():
    _initialize()


if __name__ == "__main__":
    if "/" in sys.argv[0]:
        print("ERROR: Please run this script inside of src_python_tests/! There are relative paths defined in this code that need to be respected!")
    else:
        main()
        pprint("Files generated successfully")
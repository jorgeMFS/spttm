

#!python
#!/usr/bin/env python
from curses import savetty
import os
import sys
from os import listdir, path, makedirs
from os.path import isfile, join, getsize
from pprint import pprint
from collections import Counter
from functools import reduce
import itertools
import time
import subprocess

def got_bits(input_file_rules, target_file_tape):
    return subprocess.check_output(f"./run -f {input_file_rules} -t {target_file_tape} -k 2:8 -A 5000".split(" ")).decode().strip().split(",")

def get_temp_file_for_best_rule(f, alphabet, state):
    with open(f) as f:
        rule_matrix = f.readline().split("\t")[0]
        
    with open("_temp_f", "w") as fOut:
        fOut.write(f"{rule_matrix}\t{alphabet}\t{state}\t1000")
    
    return "_temp_f"

#Paths
working_dir="../"
save_csv_path="csv"
# Variable
CSV_FILE="search_results"
def _initialize():
    os.chdir(working_dir)
    if not os.path.exists(save_csv_path):
        os.mkdir(save_csv_path)
    with open(join(save_csv_path, CSV_FILE+".csv"), 'w') as csvOut:
        writeCSVLine(csvOut,['Alphabet','States', 'Search', 'NC','File Name', 'Min loss', 'C(y|x)', 'C(y)'])
        for p in ['results_sequential/', 'results_monte_carlo/','results_guided/']:  # 'results_treesearch/'
            for alphabet in range(2,5,2):
                for state in [ 2**x for x in range(1,7)]:
                    path_s=  p+str(alphabet)+"/"+str(state)+"/"
                    sModes = [x[0].split("/")[-1] for x in os.walk(path_s)][1:]
                    for mode in sModes:
                        for nc in range(0,10,2):
                            path = path_s+mode+"/"+str(nc)+"/"
                            if os.path.exists(path):
                                for filename in filter(lambda x: not x.startswith("test"), os.listdir(path)):
                                    f = os.path.join(path, filename)
                                    rule_matrix_file_path = get_temp_file_for_best_rule(f, alphabet, state)
                                    fl_result = _process_file(f)
                                    writeCSVLine(csvOut,[str(alphabet),str(state),mode, str(nc/10),filename, fl_result]+got_bits(rule_matrix_file_path, os.path.join("tapeTestPool", str(alphabet), str(state), str(nc), filename)))
def main():
    _initialize()
def _process_results(listOfResults):
    numberPrograms=[lst[0] for lst in listOfResults]
    avgNumber=reduce(lambda a, b: a + b, numberPrograms) / len(numberPrograms)
    lossValues=[lst[1] for lst in listOfResults]
    avgLoss=reduce(lambda a, b: a + b, lossValues) / len(lossValues)
    programsFound=[lst[2] for lst in listOfResults].count(True)
    return [str(avgNumber), str(avgLoss), str(programsFound)]
def _process_file(filepath):
    numberOfResults=0
    averageLoss=1
    found=False
    with open(filepath) as f:
        lines = f.readlines()
    lossResults=[float(line.replace('\n','').split('\t')[1]) for line in lines]
    return str(lossResults[0])
def writeCSVLine(f, line):
    f.write(",".join(line))
    f.write("\n")

if __name__ == "__main__":
    if "/" in sys.argv[0]:
        print("ERROR: Please run this script inside of src_python_tests/! There are relative paths defined in this code that need to be respected!")
    else:
        main()
        pprint("Files generated successfully")
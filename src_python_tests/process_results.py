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

#Paths
working_dir="../"
save_csv_path="csv"

# Variable
CSV_FILE="search_results"

def _initialize():
    os.chdir(working_dir)
    if not os.path.exists(save_csv_path):
        os.mkdir(save_csv_path)
    writeCSVLine(['Alphabet','States', 'Search', 'Average N. Results', 'Average Loss', 'Programs Found'])
    for alphabet in range(2,5):
        for state in range(2,31):
            file_results = []
            path_s=  "results/"+str(alphabet)+"/"+str(state)+"/"
            sModes = [x[0].split("/")[-1] for x in os.walk(path_s)][1:]
            for mode in sModes:
                path = "results/"+str(alphabet)+"/"+str(state)+"/"+str(mode)+"/";           
                for filename in os.listdir(path):
                    f = os.path.join(path, filename)
                    fl_result = _process_file(f)
                    if fl_result[0]>0:
                        file_results.append(fl_result)
                results=_process_results(file_results)
                writeCSVLine([str(alphabet),str(state),mode]+results)



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
    numberOfResults=len(lossResults)
    if lossResults:
        averageLoss= sum(lossResults) / len(lossResults)
    found = 0 in lossResults
    return [numberOfResults, averageLoss, found]


def writeCSVLine(line):
    fileName = CSV_FILE+".csv"
    f = open(join(save_csv_path, fileName), 'a')
    f.write(",".join(line))
    f.write("\n")
    f.close()

if __name__ == "__main__":
    if "/" in sys.argv[0]:
        print("ERROR: Please run this script inside of src_python_tests/! There are relative paths defined in this code that need to be respected!")
    else:
        main()
        pprint("Files generated successfully")
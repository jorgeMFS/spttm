# spttm

## RUN
Run program
```
make clean all
```

## RUN
Flags to run code:
```

-a: alphabet size
-s: number of states
-i: turing tape iterations 
-k: context range for markov models
-A: alpha of markov model
-f: file
-e: seed
-o: number of output lines per run
-N: number of files
-T: nrc threshold
-S: search mode (sequential, monte carlo and neural networks)
```
  
Example of runing sptm:
```
./sptm   -e 555 -s 2 -a 2 -i 100 -A 20 -k 2 -f "A" -o 20
```

Input to program while running code:
```
0-2-2-0-1-6-1-2-6-2-2-0-2-2-6-1-2-7-1-0-3-1-1-2-2-1-5-0-0-6-3-2-4-2-0-6-1-1-0-2-0-6-3-0-5-0-2-1-3-1-3-3-0-2-0-2-0-2-0-1-0-0-0-1-2-5-1-0-1-2-2-1-1-0-7-0-0-0-3-0-6-2-0-1-3-2-4-0-2-2-2-2-4-2-1-0-	4	8	11252	2:7	20	7568
```

```
0-2-2-(...)-4-2-1-0-: rule matrix
4: alphabet size
8: number of states
11252: turing tape iterations 
7: context to markov model
20: alpha of markov model
7568:seed
```

Outputs:
The outputs from the program provide every 10 iterations of the Turing Machine, the following features:

- NRC: Normalize Relative Compression (for a given k)
- DTA: Normalize Absolute Difference between sizes of the Input and generated Turing Machine Tape
- HS: Hit Score element by element between the Input and generated Turing Machine Tape
- ED: Euclidean Distance between the Input and generated Turing Machine Tape
- PEL: Variation in diference for each alphabet letter between the Input and generated Turing Machine Tape
```
NRC DTA HS  ED  PEL
```
E.g.
```
1.88525 0.419048        0.639344        3.48925e+10     0.362538        0.0680838
```

Example of runing tprl:
```
./tprl   -e 555 -s 2 -a 2 -i 100 -A 20 -k 2 -f "A" -o 20 -N 10000
```

Outputs: files in dataset folder

Example of runing nrc:
```
./nrc -a 2  -A 1 -k 2:9 -f "tapes/A" -t "tapes/B"
```


Example of runing spark:
```
./spark   -e 777 -s 4 -a 2 -i 1000 -l 0.001 -k 2:3 -T 0 -S S -t "tapeTestPool/A_1"
```


## ISSUES
Please let us know if there is any
[issues](https://github.com/jorgeMFS/SPPTM/issues).


## LICENSE
SPPTM is under GPL v3 license. For more information, click
[here](http://www.gnu.org/licenses/gpl-3.0.html).
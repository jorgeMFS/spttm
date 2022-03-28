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
-k: context to markov model
-A: alpha of markov model
-f: file
-e: seed
```
  
Example of runing code:
```
./sptm   -e 555 -s 2 -a 2 -i 100 -A 20 -k 2 -f "A"
```

Input to program while running code:
```
0-2-2-0-1-6-1-2-6-2-2-0-2-2-6-1-2-7-1-0-3-1-1-2-2-1-5-0-0-6-3-2-4-2-0-6-1-1-0-2-0-6-3-0-5-0-2-1-3-1-3-3-0-2-0-2-0-2-0-1-0-0-0-1-2-5-1-0-1-2-2-1-1-0-7-0-0-0-3-0-6-2-0-1-3-2-4-0-2-2-2-2-4-2-1-0-	4	8	11252	7	20	7568
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
## ISSUES
Please let us know if there is any
[issues](https://github.com/jorgeMFS/SPPTM/issues).


## LICENSE
SPPTM is under GPL v3 license. For more information, click
[here](http://www.gnu.org/licenses/gpl-3.0.html).
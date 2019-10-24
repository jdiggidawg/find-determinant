Find determinant
================

Find the determinant of the given NxN matrix, where N>2,using multithreading.


### Compile find_det.c
-----------------------
```
gcc --std=c99 -Wall -g -Werror find_det.c -o find_det -lpthread
```

### Running the program
```
./find_det
```

### How to interact with the program
------------------------------------
Running the program will first output the determinants for 2 examples. Then prompt the user to enter the size for a randomly generated NxN matrix.

![alt text](https://github.com/jdiggidawg/find-determinant/blob/master/demo.png "demo")

### Abstract
------------
The determinant of a matrix can be determined by calculating the determinant of
its submatrices until N<=2. Divide the task of calculating the determinant of
submatrices by creating multiple threads and joining the results at the end to
compute the complete determinant.

### Notes about the program:
----------------------------
- The entries in the matrix are between -1000 and 1000.
- In general, N > 7 is not recommended. Since the OS probably deems too many threads are created, you will most likely see Segmentation fault and program quitting.



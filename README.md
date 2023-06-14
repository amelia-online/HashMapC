# HashMapC
A sort-of generic implementation of a HashMap in C.

## Disclaimer

This may be an imperfect/flawed implementation. I cannot guarantee its functionality.

## Usage

In order to use this implementation, the user must provide some functions for the HashMap to use, with some exceptions.
If your keys are strings or ints, I have provided some functions for that case (see main.c). Otherwise, the user must provide
a hash function, key-equality function, value allocation function, and value deallocation function.

## How to make example executable (Main) and library

Download a .zip of this project and ```cd``` into it. Use ```make``` to automatically build the executable (Main) and
```make test``` to run it; or alternatively, ```./Main```.
If you want to make a library from the .o files, simple enter ```make libhashmap.a```.
Finally, to clean up all files made by the compiler, enter ```make clean```.

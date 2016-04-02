# C-Plus-Plus-Code-Sample
Queues - Application: Very Long Integer

Programming Language: C++

Purpose
Using Deque, this programming assignment implements a LongInt class to facilitate very long integers that cannot be represented with ordinary C++ int variables.

Very Long Integers
Some real-world applications such as public/private-key cryptography need to use integers more than 100 digits long. Those integer values are obviously not maintained in C++ int variables. In this programming assignment, you will design a LongInt class that maintains a very long integer with a Deque of charaters, (i.e., chars). In other words, each char-type item in Deque represents a different digit of a given integer; the front item corresponds to the most significant digit; and the tail item maintains the least significant digit. The following code shows your LongInt's header file, (i.e., longint.h).


![Alt text](https://github.com/mahokyin/C-Plus-Plus-Code-Sample-01/blob/master/output.png "Optional Title")

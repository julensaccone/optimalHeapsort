# Research and implementation of Optimal Heapsort algoritmh

---

This is a research performed by me for the University of Milan and object of my thesis of bachelor graduation.
The thesis is about the study of an in-place sorting algoritmh that is better in time of complexity compared to ever other in-place algorythm.
This particular algorithm uses internally a better version of the famous heapsort algorithm, which is called Optimal Heapsort.
In this project i report my implementation of said algorithm using C++ language.

There are three version of the program i wrote that respectively works with array of int, double and strings.
In every program version there's not only the implementatoin of Optimal Heapsort but there's also the implementation of Classic Heapsort.
This is because the final goal is to compare both the algorithms and determine which is better.
I personally tested the program and I can tell that the Optimal Heapsort is more efficient than the classic Heapsort in terms of number of comparison and number of element moves.
When it comes to time of execution the Optimal Heapsort is faster than Classic Heapsort only if the cost of the comparison between data becomes to be 'heavy'.
For this reason in the version of integers and double the Optimal Heapsort has higher execution time.
To solve this issue I added a function timeWaster() that makes the comparison heavier by wasting some time executing few useless statements.
This is not the best solution cause we don't want to add useless statements that waste time in a real situation where we want to sort some int elements.
Instead, in the version that works on array of strings I didn't need to add this time waster function cause the Optimal Heapsort by itself has an average execution time lower than Classic Heapsort.

## Compilation and execution

---

The testing was made mainly on macOS Monterey 12.1 but also on Windows 10, having similar results.
On macOS I recommend using clang++ as compiler, but you can use also g++. Search the better compiler for our platform.
In any case always remember to compile disablig compiler's optimization otherwise you won't see the correct results in terms of execution time cause the compiler try to optimize trasforming, rearranging or deleting some blocks of your code. To disable compiler optimization the option '-O0' should be enough.
Also remember to put '-std=c++20' otherwise some functionality in the code might not work.

To compile on macOS use:

```
clang++ -O0 -fstandalone-debug -std=c++20  heapsortMigliorato_v7.cpp -o heapsortMigliorato_v7
```

Since Optimal Heapsort has been proved as a better version of the classic Heapsort, by using it in the main In-Place Sortig algorithm this gets more efficient than it was before.
A next step in this research would be to figured out why on the integer version and in the double version, without the timeWaster() function, the Optimal Heapsort has higher execution time even if it's more efficient in terms of numeber of comparison and element moves.

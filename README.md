# Covering Arrays

This program will verify a covering arrays and identify "don't care" positions.

# Input

The input is always rad from `stdin` and is expected to be in the following
format:

```
N t k v
1 0 ... k
.
.
.
N       Nk

where:

N - number of rows
t - number of columns per t-set
k - number of columns
v - number of symbols
```

# Output

A decision "true" or "false" on whether the input array is a covering array.  If
the array is a covering array, then print a map of the "don't care" positions as
a table then as (r,c) pairs.

# Design and Analysis

### 1. Discuss selection of data structures for generating subarrays and t-sets.

All of the combinations of subarrays were held in a large vector of t element
arrays. Something like:

```
[0,1,2]
.......
.......
[2,3,4]
```

Where the rows contain indexes to the columns of the covering array. The were
calculated before checking the covering array.

Moreover, permutations of symbols were also calculated, but only when needed.

### 2. Discuss the data structure used to keep track of coverage conditions in the CA.

The main data structure in this project is:

```
// struct to represent Covering Array
typedef struct {
  int N; // number of rows
  int t; // number of columns per t-set
  int k; // number of columns
  int v; // number of symbols
  int **data; // 2d array for data
  int **dc; // used for storing don't care info
} CA;
```

It holds all the information from the input file. The 2d-array `data` field is
dynamically allocated because its not know until run time, all the other fields
are fixed in size.

### 3. Discuss the data structure used to keep track of "don't care" positions.

Alone with the covering array, the CA structure also contains a `dc` field to
represent all the don't care positions. As we traverse the data 2d-array  with
respect to combinations of columns and permutations of symbols, I keep
track/count the number of occurrences in the `dc` 2d-array. If any elements are
0, then the input in not a covering array, if any of the positions are 1, they
can't be don't cares.

# Test

To run all of the tests in the test directory:

    $ make test

The project contains an output.txt as a sample of the output the test command
produces.

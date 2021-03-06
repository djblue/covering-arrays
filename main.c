#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <omp.h>

#include "vector.h"

// struct to represent Covering Array
typedef struct {
  int N; // number of rows
  int t; // number of columns per t-set
  int k; // number of columns
  int v; // number of symbols
  int **data; // 2d array for data
  int **dc; // used for storing don't care info
} CA;

// display covering array struct, good for debugging
void print_ca (CA *ca) {

  // print metadata
  printf("CA(N:%d,t:%d,k:%d,v:%d)\n", ca->N, ca->t, ca->k, ca->v);

  // print rows and columns
  for (int i = 0; i < ca->N; i++) {
    for (int j = 0; j < ca->k; j++) {
      printf("%d ", ca->data[i][j]);
    }
    printf("\n");
  }

}

// print out the don't care positions for the covering array
void print_dc (CA *ca) {

  // print them as a 2d-array
  for (int i = 0; i < ca->N; i++) {
    for (int j = 0; j < ca->k; j++) {
      if (ca->dc[i][j] == 0) {
        printf("x ");
      } else {
        printf("%d ", ca->data[i][j]);
      }
    }
    printf("\n");
  }

  // print them as 2-tuples
  for (int i = 0; i < ca->N; i++) {
    for (int j = 0; j < ca->k; j++) {
      if (ca->dc[i][j] == 0) {
        printf("(%d,%d)\n", i, j);
      }
    }
  }

}

// allocate 2d array for use in the Covering Array
// r - number of rows
// c - number of columns
int **make_data (int r, int c) {
  int **data;
  data = (int**) malloc(r * sizeof(int*));
  for (int i = 0; i < r; i++) {
    data[i] = (int*) malloc(c * sizeof(int));
  }
  return data;
}

// clear allocated data for Covering Array
// data - pointer to data
// r - number of rows
void free_data (int** data, int r) {
  for(int i = 0 ; i < r; i++)
    free(data[i]);
  free(data);
}

// read input from stdin
// format specified in readme
void read_input (CA *ca) {

  // read ca metadata
  scanf("%d", &ca->N);
  scanf("%d", &ca->t);
  scanf("%d", &ca->k);
  scanf("%d", &ca->v);

  // read ca data

  // since data is dynamic, needs to be heap allocated
  ca->data = make_data(ca->N, ca->k);
  ca->dc = make_data(ca->N, ca->k);

  // read rows
  for (int i = 0; i < ca->N; i++) {
    for (int j = 0; j < ca->k; j++) {
      ca->dc[i][j] = 0;
      scanf("%d", &ca->data[i][j]);
    }
  }

}

// not used but was the originally used to test calculating
// different permutations, leaving for reference
// t - the number of columns in the table
// v - number of symbols in the table
vector *calc_perm (int t, int v) {

  int rows = pow(v,t);

  int *row = (int*)malloc(t*sizeof(int));
  vector *vec = vector_create(t*sizeof(int));

  // columns
  for (int i = 0; i < rows; i++) {
    // rows
    for (int j = t - 1; j >= 0; j--) {
      row[j] = ( i / ((long)pow(v,j)) ) % v;
    }
    vector_push(vec, row);
  }

  free(row);

  return vec;
}

// enumerate all the combinations in a vector
// make sure to free the vector when done
// its a bit messy but it works
vector *calc_comb (int k, int t) {

  // allocate space for my vector and other stuffs
  // using a vector is nice because then I don't have
  // to pre calculate the size of the array that is
  // needed, when I did that, the factorials involved
  // would give me division/floating point exceptions
  // for large enough k.
  vector *v = vector_create(t * sizeof(int));
  int *last_row = (int*) malloc(t * sizeof(int));
  int *very_last_row = (int*) malloc(t * sizeof(int));

  // set the last row
  for (int j = 0; j < t; j++) {
    very_last_row[j] = k - (t - j);
  }

  // calculate the different rows in the combination list
  int i;
  for (i = 0; last_row[0] != k - t; i++) {

    if (i == 0) {
      for (int j = 0; j < t; j++) {
        last_row[j] = j;
      }
    } else {
      // this is just a huge mess, i don't know how this happened but
      // much like other c code, it seems to work, so im just going
      // to leave it here and try not to anger it.
      for (int j = t - 1; j > -1; j--) {
        if (last_row[j] < k - 1 && last_row[j] < very_last_row[j]) {
          last_row[j] = last_row[j] + 1;
          for (int k = j + 1; k < t; k++) {
            last_row[k] = last_row[k-1] + 1;
          }
          break;
        }
      }

    }

    // push the row into the vector
    vector_push(v, last_row);
  }


  free(very_last_row);
  free(last_row);

  return v;
}

// grab value from different columns and compact them into
// a single array this is more for sanity, then for performance
// table - the table 2d-array to grab the values from
// row - the row number in the table
// cols - an array of all the columns to pluck from
// no_of_cols - number of columns in cols
// out - an output array to write to
void pluck_cols_from_row (int **table, int row, int *cols,
                          int no_of_cols, int*out) {
  for (int i = 0; i < no_of_cols; i++) {
    out[i] = table[row][cols[i]];
  }
}

// determine if two rows are equivalent
// row1, row2 - two arrays of ints
// no_of_cols - number of elements to check in the array
// NOTE: no bounds checking, could fault
int row_equal (int *row1, int *row2, int no_of_cols) {
  for (int i = 0; i < no_of_cols; i++) {
    if (row1[i] != row2[i]) {
      return 0;
    }
  }
  return 1;
}

// check single column combination and a single symbol permutation
// ca - an initialized covering array
// cols - the current combination of columns
void check (CA *ca, int *cols, int *perm) {

  // the permutation of values
  int *temp = (int*) malloc(ca->t * sizeof(int));

  int count = 0;
  int last_find;

  // check every row in covering array
  for (int j = 0; j < ca->N; j++) {

    // grab all the row valus from the different columns and
    // compact them in to a continuous row for easy comparison
    pluck_cols_from_row(ca->data, j, cols, ca->t, temp);

    if (row_equal(perm, temp, ca->t)) {
      count++;
      last_find = j;
    }

  }

  // t-set doesn't have permutation, not a covering array
  // inform user, and exit
  if (count == 0) {
    #pragma omp critical
    {
      // don't print false more than once
      printf("false\n");
      exit(0);
    }
  }

  // useful for don't cares
  // if the permutation was only found once, mark it in the
  // don't care 2d-array (dc)
  if (count == 1) {
    for (int k = 0; k < ca->t; k++) {
      #pragma omp critical
      ca->dc[last_find][cols[k]]++;
    }
  }

  // free dynamic memory
  free(temp);
  
}

int main () {

  CA ca;

  // read input from stdin
  read_input(&ca);

  // calculate all the combinations of the columns in the covering
  // array.
  vector *comb = calc_comb(ca.k, ca.t);
  vector *perm = calc_perm(ca.t, ca.v);

  // check all the combination and permutations
  #pragma omp parallel for shared(ca,comb,perm)
  for (int i = 0; i < comb->length; i++) {
    for (int j = 0; j < perm->length; j++) {
      check(&ca, vector_get(comb, i), vector_get(perm, j));
    }
  }

  // if the program hasn't exited yet, then the input
  // is a covering array
  printf("true\n");
  print_dc(&ca);

  // free all the dynamic memory
  vector_free(comb);
  vector_free(perm);

  free_data(ca.data, ca.N);
  free_data(ca.dc, ca.N);

  return 0;
}

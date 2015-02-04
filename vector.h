#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_INITIAL_CAPACITY 64
#define VECTOR_GROWTH_RATE 1.5

typedef struct {
  void *data;   // pointer to data of vector
  int unit;     // unit size of any element in vector
  unsigned long length;   // how many element are currently stored
  unsigned long capacity; // how many spaces of unit size are available
} vector;

// create a new vector
vector *vector_create(int unit);
// free vector resources
void vector_free(vector *v);

// push to back of vector 
void vector_push(vector *v, void *elem);
// pop last element
void *vector_pop(vector *v);
// get element i, returns null if out of bounds
void *vector_get(vector *v, int i);
// splice out n elements from index i
void vector_splice(vector *v, int i, int n);

// apply a function to each element in array
void vector_each(vector *v, void func (void *));
// get a new vector filtered from v
vector *vector_filter(vector *v, int func (void *));
// get a new vector mapped from v
vector *vector_map(vector *v, int unit, void *func (void *));

#endif

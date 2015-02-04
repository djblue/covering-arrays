#include <stdlib.h>
#include <string.h>

#include "vector.h"

vector *vector_create(int unit) {

  vector *v = (vector*) malloc(sizeof(vector));

  if (v == NULL) return NULL;

  v->data = malloc(unit*VECTOR_INITIAL_CAPACITY);
  v->unit = unit;
  v->length = 0;
  v->capacity = VECTOR_INITIAL_CAPACITY;

  return v;
}

void vector_free(vector* v) {
  free(v->data);
  free(v);
}

void vector_resize (vector *v) {
  int cap = (int)(v->capacity * VECTOR_GROWTH_RATE);
  void *p = malloc(cap * v->unit);
  memcpy(p, v->data, v->length * v->unit);
  free(v->data);
  v->data = p;
  v->capacity = cap;
}

void vector_push(vector *v, void *elem) {

  if (v->length == v->capacity) {
    vector_resize(v);
  }

  // copy over element
  memcpy(v->data + (v->unit * v->length), elem, v->unit);

  v->length++; // increase length of vector
}

void *vector_pop(vector* v) {

  // no elements left to pop
  if (v->length == 0) return NULL;

  // change length of vector
  v->length--;

  // return address of last element
  return v->data + (v->unit * v->length);
}

void *vector_get(vector *v, int i) {
  // i is out of range
  if  (i < 0 || i >= v->length) return NULL;

  return v->data + (i * v->unit);
}

void vector_splice(vector *v, int i, int n) {

  int max;
  void *start = v->data + (v->unit * i);

  // n should not exceed max
  max = v->length - i;
  if (n > max) n = max;

  memmove(start, start + (n * v->unit), v->unit * (v->length - n));

  v->length -= n; // remove n elements
}

void vector_each(vector *v, void func (void *)) {
  int i;
  void *p;
  for (i = 0; i < v->length; i++) {
    p = v->data + (i * v->unit);
    func(p);
  }
}

vector *vector_filter(vector *v, int func (void *)) {

  int i, res;

  vector *filter = vector_create(v->unit);

  for (i = 0; i < v->length; i++) {
    res = func(v->data + (i * v->unit));
    if (res == 1) {
      vector_push(filter, v->data + (i * v->unit));
    }
  }

  return filter;
}

vector *vector_map(vector *v, int unit, void *func (void *)) {

  int i;
  void *res;

  vector *map = vector_create(unit);

  for (i = 0; i < v->length; i++) {
    res = func(v->data + (i * v->unit));
    vector_push(map, res);
    free(res);
  }

  return map;
}

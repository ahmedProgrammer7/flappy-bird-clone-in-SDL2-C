#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
  float x;
  float y;
} Vector;

void vector_add(Vector* to, const Vector v);
void vector_sub(Vector* from, const Vector v);

#endif // VECTOR_H

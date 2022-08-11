#include "../include/vector.h"

void vector_add(Vector* to, const Vector v)
{
  to->x += v.x;
  to->y += v.y;
}

void vector_sub(Vector* from, const Vector v)
{
  from->x -= v.x;
  from->y -= v.y;
}

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include "vector.h"

typedef struct {
  float w;
  float h;
} Size;

typedef struct {
  float w;
  float h;
} Scale;

typedef struct {
  Vector pos;
  Size size;
  Scale scale;
  SDL_Texture* texture;
} Entity;

void render_entity(SDL_Renderer* renderer, Entity* entity);
void render_entities(SDL_Renderer* renderer, Entity entities[], int length);

#endif // ENTITY_H

#include "../include/entity.h"

void render_entity(SDL_Renderer* renderer, Entity* entity)
{
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = (entity->size).w;
  src.h = (entity->size).h;

  SDL_Rect dst;
  dst.x = (entity->pos).x;
  dst.y = (entity->pos).y;
  dst.w = (entity->size).w * (entity->scale).w;
  dst.h = (entity->size).h * (entity->scale).h;

  SDL_RenderCopy(renderer, entity->texture, &src, &dst);
}

void render_entities(SDL_Renderer* renderer, Entity entities[], int length)
{
  for (int i = 0; i < length; i++)
  {
    render_entity(renderer, &entities[i]);
  }
}

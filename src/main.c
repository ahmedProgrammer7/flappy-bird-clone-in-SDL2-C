#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/entity.h"
#include "../include/vector.h"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 576;

int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("Error: Initializing SDL.\n%s\n", SDL_GetError());
    exit(1);
  }

  if (!IMG_Init(IMG_INIT_PNG))
  {
    printf("Error: Initializing SDL_image.\n%s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  SDL_Window* window;
  window = SDL_CreateWindow(
    "flappy bird",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if (window == NULL)
  {
    printf("Error: Creating window.\n%s\n", SDL_GetError());
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  SDL_Renderer* renderer;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL)
  {
    printf("Error: Creating renderer.\n%s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  SDL_Texture* background_texture;
  SDL_Texture* bird_texture;
  SDL_Texture* pipe_top_texture;
  SDL_Texture* pipe_bottom_texture;
  background_texture = IMG_LoadTexture(renderer, "res/background.webp");
  bird_texture = IMG_LoadTexture(renderer, "res/bird.xcf");
  pipe_top_texture = IMG_LoadTexture(renderer, "res/pipe_top.xcf");
  pipe_bottom_texture = IMG_LoadTexture(renderer, "res/pipe_bottom.xcf");
  if (background_texture == NULL || bird_texture == NULL || pipe_top_texture == NULL || pipe_bottom_texture == NULL)
  {
    printf("Error: Loading texture.\n%s\n", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    exit(1);
  }

  Entity background;
  background.pos.x = 0;
  background.pos.y = 0;
  background.size.w = 1024;
  background.size.h = 768;
  background.scale.w = 1.0f;
  background.scale.h = 0.75f;
  background.texture = background_texture;

  const int PIPES_LENGTH = 20;
  Entity pipes[PIPES_LENGTH];
  srand(time(NULL));
  const int DISTANCE = 400;
  const int HOLE = 200;
  int min = -WINDOW_HEIGHT;
  int max = 0 - HOLE;

  // create top pipes
  for (int i = 0; i < PIPES_LENGTH / 2; i++)
  {
    Entity pipe;
    pipe.size.w = 17;
    pipe.size.h = 115;
    pipe.scale.w = 5.0f;
    pipe.scale.h = 5.0f;
    pipe.pos.x = WINDOW_WIDTH + (i * DISTANCE);
    pipe.pos.y = (rand() % (max - min + 1)) + min;
    pipe.texture = pipe_top_texture;
    pipes[i] = pipe;
  }

  // create bottom pipes
  for (int i = PIPES_LENGTH / 2, j = 0; i < PIPES_LENGTH; i++, j++)
  {
    Entity pipe;
    pipe.size.w = 17;
    pipe.size.h = 115;
    pipe.scale.w = 5.0f;
    pipe.scale.h = 5.0f;
    pipe.pos.x = WINDOW_WIDTH + (j * DISTANCE);
    pipe.pos.y = pipes[j].pos.y + (pipes[j].size.h * pipes[j].scale.h) + HOLE;
    pipe.texture = pipe_bottom_texture;
    pipes[i] = pipe;
  }

  Entity* farest_pipe = &pipes[PIPES_LENGTH - 1];

  Entity bird;
  bird.size.w = 17;
  bird.size.h = 12;
  bird.scale.w = 5.0f;
  bird.scale.h = 5.0f;
  bird.pos.x = WINDOW_WIDTH / 2 - (bird.size.w * bird.scale.w) / 2;
  bird.pos.y = WINDOW_HEIGHT / 2 - (bird.size.h * bird.scale.h) / 2;
  bird.texture = bird_texture;

  Vector vel = { 0.0f, 0.0f };
  Vector acc = { 0.0f, 0.0f };

  bool jump = false;

  bool quit = false;
  SDL_Event event;

  while (!quit)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode)
          {
            case SDL_SCANCODE_SPACE:
              jump = true;
              break;
          }
          break;
        case SDL_KEYUP:
          switch (event.key.keysym.scancode)
          {
            case SDL_SCANCODE_SPACE:
              jump = false;
              break;
          }
          break;
      }
    }

    for (int i = 0; i < PIPES_LENGTH; i++)
      pipes[i].pos.x -= 4.5f;
    
    // reset pipe position after getting out of screen
    for (int i = 0; i < PIPES_LENGTH / 2; i++)
    {
      if (pipes[i].pos.x < -(pipes[i].size.w * pipes[i].scale.w))
      {
        pipes[i].pos.x = (farest_pipe->pos).x + DISTANCE;
        pipes[i + PIPES_LENGTH / 2].pos.x = (farest_pipe->pos).x + DISTANCE;
        pipes[i].pos.y = (rand() % (max - min + 1)) + min;
        pipes[i + PIPES_LENGTH / 2].pos.y = pipes[i].pos.y + (pipes[i].size.h * pipes[i].scale.h) + HOLE;
        farest_pipe = &pipes[i];
        break;
      }
    }

    // check pipe collision with bird
    for (int i = 0; i < PIPES_LENGTH / 2; i++)
    {
      if (bird.pos.x + (bird.size.w * bird.scale.w) >= pipes[i].pos.x &&
        bird.pos.x <= pipes[i].pos.x + (pipes[i].size.w * pipes[i].scale.w) &&
        (!(bird.pos.y >= pipes[i].pos.y + (pipes[i].size.h * pipes[i].scale.h)) ||
        !(bird.pos.y + (bird.size.h * bird.scale.h) <= pipes[i].pos.y + (pipes[i].size.h * pipes[i].scale.h) + HOLE)))
      {
        quit = true;
      }
    }

    // check window screen collision with bird
    if (bird.pos.y + (bird.size.h * bird.scale.h) >= WINDOW_HEIGHT)
    {
      bird.pos.y = WINDOW_HEIGHT - (bird.size.h * bird.scale.h);
      vel.y = 0;
      acc.y = 0;
    }
    else if (bird.pos.y <= 0)
    {
      bird.pos.y = 0;
      vel.y = 0;
      acc.y = 0;
    }

    acc.y = jump ? -3.2f : 0.85f;

    vector_add(&vel, acc);
    vector_add(&bird.pos, vel);

    SDL_RenderClear(renderer);
    render_entity(renderer, &background);
    render_entity(renderer, &bird);
    render_entities(renderer, pipes, PIPES_LENGTH);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(pipe_bottom_texture);
  SDL_DestroyTexture(pipe_top_texture);
  SDL_DestroyTexture(bird_texture);
  SDL_DestroyTexture(background_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}

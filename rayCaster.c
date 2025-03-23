#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

SDL_Window* gWindow = NULL;
SDL_Renderer* Renderer = NULL;

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

bool init(void) {
   bool success = SDL_Init(SDL_INIT_VIDEO);
   if( !success ) {
      SDL_Log("Initialization failed! Cuz: %s\n", SDL_GetError());
   }

   return success;
}

bool makeWindow(void) {
  gWindow = SDL_CreateWindow("RayCaster", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  Renderer = SDL_CreateRenderer(gWindow, NULL);
  
  if(gWindow == NULL) {
    SDL_Log("Window creation failed! Cuz: %s\n", SDL_GetError()); 
    return false;
  }

   return true;
}

void end(void){
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  Renderer = NULL;
  SDL_Quit();
}

//--------------------------------------------------------------------------
#define MAP_WIDTH 16
#define MAP_HEIGHT 8

int WorldMap[MAP_HEIGHT][MAP_WIDTH] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1},
  {1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1},
  {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

struct entity{
  double posX, posY;
  double dirX, dirY;
  double planeX, planeY;
};

void renderFrame(){
  SDL_SetRenderDrawColor(Renderer, 80, 120, 250, 255);
  SDL_RenderClear(Renderer);

  SDL_SetRenderDrawColor(Renderer, 100, 200, 100, 255);
  SDL_FRect floor = {0, SCREEN_HEIGHT / 2.0f, (float)SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f};
  SDL_RenderFillRect(Renderer, &floor);
  
  for(int i=0; i < SCREEN_WIDTH; i++){

  }

  SDL_RenderPresent(Renderer);
}


void loop(){
bool quit = false;
SDL_Event e;
SDL_zero(e);

while(!quit){
  while( SDL_PollEvent( &e ) ) {
  if( e.type == SDL_EVENT_QUIT ) quit = true;
  }
  renderFrame();
 
  SDL_UpdateWindowSurface(gWindow);
}
}

int main(void){
  if(!init())
    return -1;

  if(!makeWindow())
    return -2;



  loop();

  end();
  return 0;
}

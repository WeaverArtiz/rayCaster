#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <math.h>
#include <stdatomic.h>
#include <stdio.h>

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
#define RENDER_DISTANCE 16.0f
#define MOVE_SPEED 0.3f

bool WorldMap[MAP_HEIGHT][MAP_WIDTH] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
  {1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

float posX = 4, posY = 4;
float angle = 0, fov = 3.141592 / 4.0;

void renderFrame(){
  SDL_SetRenderDrawColor(Renderer, 0, 20, 100, 255);
  SDL_RenderClear(Renderer);

  SDL_SetRenderDrawColor(Renderer, 20, 100, 30, 255);
  SDL_FRect floor = {0, SCREEN_HEIGHT / 2.0f, (float)SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f};
  SDL_RenderFillRect(Renderer, &floor);
  
  for(int i=0; i < SCREEN_WIDTH; i++){
    float rayAngle = (angle - fov / 2.0f) + ((float)i / (float)SCREEN_WIDTH) * fov;
    float rayDistance = 0.01f;
    bool rayHit = false;
    
    float stepX = sinf(rayAngle);
    float stepY = cosf(rayAngle);
    
    while(!rayHit && rayDistance < RENDER_DISTANCE){
      rayDistance += 0.01f;

      int rayX = (int)(posX + rayDistance * stepX);
      int rayY = (int)(posY + rayDistance * stepY);
      
      if(rayX < 0 || rayX > MAP_HEIGHT || rayY < 0 || rayY > MAP_WIDTH){
        rayHit = true;
        }

      else if(WorldMap[rayX][rayY] == 1) rayHit = true;
    
    }
    int dCeiling = (int)(SCREEN_HEIGHT/2.0f + SCREEN_HEIGHT / (rayDistance + 1.5f)); 
    int dFloor = SCREEN_HEIGHT - dCeiling;
    float shade = 1 - (rayDistance / RENDER_DISTANCE);
    SDL_SetRenderDrawColor(Renderer, (int)(170 * shade), (int)(50 * shade), (int)(50 * shade), 255);
    SDL_RenderLine(Renderer, i, dFloor, i, dCeiling);
  }

  SDL_RenderPresent(Renderer);
}
void moveF(){
  float stepX = sinf(angle);
  float stepY = cosf(angle);
  posX += MOVE_SPEED * stepX;
  posY += MOVE_SPEED * stepY;
  if(WorldMap[(int)posX][(int)posY]==1){
    posX -= MOVE_SPEED * stepX;
    posY -= MOVE_SPEED * stepY;
  }
}
void moveB(){
  float stepX = sinf(angle);
  float stepY = cosf(angle);
  posX -= MOVE_SPEED * stepX;
  posY -= MOVE_SPEED * stepY;
  if(WorldMap[(int)posX][(int)posY]==1){
    posX += MOVE_SPEED * stepX;
    posY += MOVE_SPEED * stepY;
  }
}
void loop(){
bool quit = false;
SDL_Event e;
SDL_zero(e);

while(!quit){
  while( SDL_PollEvent( &e ) ) {
  if(e.type == SDL_EVENT_QUIT) quit = true;
  if(e.type == SDL_EVENT_KEY_DOWN) switch(e.key.key){
        case SDLK_D:
          angle+=0.03;
          break;
        case SDLK_A:
          angle-=0.03;
          break;
        case SDLK_W:
          moveF();
          break;
        case SDLK_S:
          moveB();
          break;
      }
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

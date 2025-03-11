#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
//SDL_Surface* gHelloWorld = NULL; //initialize loading bitmap

bool init(void) {
   bool success = SDL_Init(SDL_INIT_VIDEO);
   if( !success ) {
      SDL_Log("Initialization failed! Cuz: %s\n", SDL_GetError());
   }

   return success;
}

bool makeWindow(void) {
  const int kScreenWidth = 640;
  const int kScreenHeight = 480;
  
  gWindow = SDL_CreateWindow("WooD", kScreenWidth, kScreenHeight, 0);
  gScreenSurface = SDL_GetWindowSurface( gWindow );
  if(gWindow == NULL) {
    SDL_Log("Window creation failed! Cuz: %s\n", SDL_GetError()); 
    return false;
  }

   return true;
}

/*bool loadMedia() {
   char* imagePath = "hello.bmp";

   gHelloWorld = SDL_LoadBMP( imagePath );

   if( gHelloWorld == NULL ) {
       SDL_Log( "Unable to load image %s! Cuz: %s\n", imagePath, SDL_GetError() );
       return false;
   }

   return true;
}
*/ //loads bitmap
void loop(){
bool quit = false;
SDL_Event e;
SDL_zero(e);

while(!quit){
  while( SDL_PollEvent( &e ) ) {
  if( e.type == SDL_EVENT_QUIT ) {
  quit = true;
  }
  }
  SDL_FillSurfaceRect(gScreenSurface,
                      NULL,
                      SDL_MapSurfaceRGB( gScreenSurface, 0x10, 0x10, 0x10));

  //SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL); // draws loaded bitmap (gHelloWorld)
  SDL_UpdateWindowSurface(gWindow);
}
}

void end(void){
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gScreenSurface = NULL;
  //SDL_DestroySurface(gHelloWorld);  //delete loaded btms
  //gHelloWorld = NULL;
  SDL_Quit();
}

int main(void){
  if(!init())
    return -1;

  if(!makeWindow())
    return -2;

  //if(!loadMedia())
  //  return -4;

  loop();

  end();
  return 0;
}

#include "sdl_subsystem.hpp"
#include "SDL.h"
#include "SDL_image.h"

sdl_subsystem::sdl_subsystem()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
}

sdl_subsystem::~sdl_subsystem()
{
    IMG_Quit();

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

#include "sdl_subsystem.hpp"
#include "SDL.h"
#include "SDL_image.h"

SdlSubsystem::SdlSubsystem()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
}

SdlSubsystem::~SdlSubsystem()
{
    IMG_Quit();

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

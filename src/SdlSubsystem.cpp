#include "SdlSubsystem.hpp"
#include "SDL.h"
#include "SDL_image.h"

SdlSubsystem::SdlSubsystem()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) m_successfulInit = false;
    if (IMG_Init(IMG_INIT_PNG) == 0) m_successfulInit = false;
}

SdlSubsystem::~SdlSubsystem()
{
    IMG_Quit();

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

#include "display.h"
#include <stdio.h>

Display :: Display(const char * title,int windowHeight,int windowWidth){

    m_Chip8Display.h = windowHeight;
    m_Chip8Display.w = windowWidth;
    m_Chip8Display.x = 0;
    m_Chip8Display.y = 0;


    m_Display = SDL_CreateWindow(title, 
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 windowWidth,
                                 windowHeight,
                                 SDL_WINDOW_SHOWN);

    if(m_Display == NULL){

        SDL_Log("Could not create Display Window ! \n");
        std::cout<<"INFO : "<<SDL_GetError();

    } else {

        m_Displaysurface = SDL_GetWindowSurface(m_Display);
        if(m_Displaysurface == NULL){

            
            SDL_Log("Could not create Display Window Surface ! \n");
            std::cout<<"INFO : "<<SDL_GetError();

        } 
    }

}

Display :: ~Display(){

    SDL_FreeSurface(m_Displaysurface);
    SDL_DestroyWindow(m_Display);

}

void Display :: InitChip8Surface(uint32_t * display,int displayWidth,int displayHeight){

    m_Chip8surface = SDL_CreateRGBSurfaceFrom(display,displayWidth,
                                              displayHeight,32,
                                              displayWidth * sizeof(uint32_t),
                                              0xff000000,0x00ff0000,0x0000ff00,0);

    SDL_SetSurfaceRLE(m_Chip8surface,1);

}

void Display::Draw(uint32_t * display){

    SDL_LockSurface(m_Chip8surface);

    SDL_memcpy(m_Chip8surface->pixels,display,m_Chip8surface->h * m_Chip8surface->pitch);

    SDL_UnlockSurface(m_Chip8surface);

    SDL_BlitScaled(m_Chip8surface,NULL,m_Displaysurface,&m_Chip8Display);
    SDL_UpdateWindowSurface(m_Display);

}



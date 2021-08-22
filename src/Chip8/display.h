#include "SDL2/SDL.h"
#include <iostream>
#include <inttypes.h>

class Display{

private : 

    SDL_Window * m_Display;
    SDL_Surface * m_Displaysurface;
    SDL_Surface * m_Chip8surface;
    SDL_Rect m_Chip8Display;
    
public :

    Display(const char * title,int windowHeight,int windowWidth);
    void InitChip8Surface(uint32_t * display,int displayWidth,int displayHeight);
    void Draw(uint32_t * Display);
    ~Display();

};
#include "Chip8/display.h"
#include "Chip8/chip.h"
#include "Chip8/RomFileReader.h"
#include <cmath>
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640

int main(int argc,char * argv[]){

    // Initilizing the CHIP8 emulator 
    RomReader reader("./bin/roms/IBMLogo.ch8");
    Chip chip;
    SDL_Event event;
    int size = reader.ReturnRomSize();
    uint8_t * rom = reader.ReturnRom();
    

    // Loading the rom into the emulator

    chip.LoadRom(rom,size);
    delete rom;

    //Init SDL
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0){
        SDL_Log("Could not initialize SDL !");
        return EXIT_FAILURE;
    }


    //Init display

    Display display("CHIP8 EMULATOR",WINDOW_HEIGHT,WINDOW_WIDTH);
    display.InitChip8Surface(chip.GetDisplay(),64,32);


    // Emulationi loop
    while(1){

        if(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                break;
            }
        }

        chip.disassemble();
        if(chip.GetDrawFlag() == true){
            display.Draw(chip.GetDisplay());
            chip.ToggleDrawFlag();
        } 

    }

    return 0;

}


#include "chip.h"
#include <time.h>
#include <stdlib.h>

uint32_t * Chip::GetDisplay(){return m_display;}
bool Chip::GetDrawFlag(){return m_drawFlag;}
void Chip :: ToggleDrawFlag(){m_drawFlag = !(m_drawFlag);}


Chip :: Chip(){

    m_registerV = new uint8_t[16];
    m_stack = new uint16_t[16];
    m_ram = new uint8_t[4096];
    m_display = new uint32_t[64 * 32];
    m_programCounter = 0x0200;
    m_stackPointer = -1;
    m_key = 0x00;
    m_drawFlag = false;

    for(int i = 0;i<4096;i++)
        m_ram[i] = (uint8_t)0x00;

    for(int i = 0;i<64*32;i++)
        m_display[i] = (uint32_t)0x00;

    for(int i = 0; i < 16; i++)
        m_registerV[i] = 0x00;
    
    for(int i = 0; i < 16; i++)
        m_stack[i] = 0x0000;

    uint8_t hexDigits [] = {

        0xF0, 0x90, 0x90, 0x90, 0xF0, 
	    0x20, 0x60, 0x20, 0x20, 0x70, 
	    0xF0, 0x10, 0xF0, 0x80, 0xF0, 
	    0xF0, 0x10, 0xF0, 0x10, 0xF0, 
	    0x90, 0x90, 0xF0, 0x10, 0x10, 
	    0xF0, 0x80, 0xF0, 0x10, 0xF0, 
	    0xF0, 0x80, 0xF0, 0x90, 0xF0, 
	    0xF0, 0x10, 0x20, 0x40, 0x40, 
	    0xF0, 0x90, 0xF0, 0x90, 0xF0, 
	    0xF0, 0x90, 0xF0, 0x10, 0xF0, 
	    0xF0, 0x90, 0xF0, 0x90, 0x90, 
	    0xE0, 0x90, 0xE0, 0x90, 0xE0, 
	    0xF0, 0x80, 0x80, 0x80, 0xF0, 
	    0xE0, 0x90, 0x90, 0x90, 0xE0, 
	    0xF0, 0x80, 0xF0, 0x80, 0xF0, 
	    0xF0, 0x80, 0xF0, 0x80, 0x80  

    };

    for(uint8_t i = 0x00; i < 0x50 ; i++){

        m_ram[i] = hexDigits[i];

    }

    delete [] hexDigits;

}

Chip :: ~Chip(){

    delete m_registerV;
    delete m_stack;
    delete m_ram;
    delete m_display;

}

void Chip :: ClearDisplay(){
    
    for(int i = 0; i < 64*32 ; i++)
        m_display[i] = 0x00;
}

void Chip::LoadRom(uint8_t * rom,int size){

    int start = 0x200;

    for(int i = 0;i<size;i++){

        m_ram[start+i] = rom[i];

    }
    
}

void Chip::disassemble(){

    uint16_t upperNible = m_ram[m_programCounter];
    uint16_t lowerNible = m_ram[m_programCounter + 1];
    uint16_t instruction = (upperNible << 8) | (lowerNible);
    uint16_t type = (instruction & 0xF000) >> 12;
    uint16_t regaddr_x = (instruction & 0x0F00) >> 8;
    uint16_t regaddr_y = (instruction & 0x00F0) >> 4;
    uint16_t register_addr = (instruction & 0x0F00) >> 8;        
    uint16_t byte = instruction & 0x00FF;
    uint16_t addr = instruction & 0x0FFF;
    uint16_t bit = instruction & 0x000F;


    switch(type){

        case 0x0 : {
            switch(byte){

                case 0xE0 : {
                    for(int i = 0;i<64*32;i++)
                        m_display[i] = (uint32_t)0x00;
                    m_drawFlag = true;
                }

                break;

                case 0xEE : {
                    m_programCounter = m_stack[m_stackPointer];
                    m_stackPointer -= 1;
                } 
                break;

            }
        }
        break;

        case 0x1 :{
            m_programCounter = addr;
        }
        break;

        case 0x2 :{
            m_stackPointer += 1;
            m_stackPointer = m_programCounter;
            m_programCounter = addr;
        }
        break;

        case 0x3 : {
            if(m_registerV[register_addr] == byte)
                m_programCounter += 0x02;
        }
        break;

        case 0x4 : {
            if(m_registerV[register_addr] != byte)
                m_programCounter += 0x02;
        }
        break;

        case 0x5 : {
            if(m_registerV[regaddr_x] == m_registerV[regaddr_y])
                m_programCounter += 0x02;
        }
        break;

        case 0x6 : {
            m_registerV[register_addr] = byte;
        }
        break;

        case 0x7 : {
            m_registerV[register_addr] += byte;
        }
        break;

        case 0x8 : {
            switch(instruction & 0x000F){

                case 0x0 : {
                    m_registerV[regaddr_x] = m_registerV[regaddr_y];
                }                     
                break;

                case 0x1 : {
                    m_registerV[regaddr_x] = m_registerV[regaddr_x] | m_registerV[regaddr_y];
                } 
                break;

                case 0x2 : {
                    m_registerV[regaddr_x] = m_registerV[regaddr_x] & m_registerV[regaddr_y];
                } 
                break;

                case 0x3 : {
                    m_registerV[regaddr_x] = m_registerV[regaddr_x] ^ m_registerV[regaddr_y];
                } 
                break;

                case 0x4 : {
                    uint16_t add = m_registerV[regaddr_x] + m_registerV[regaddr_y];
                    if(add > 255)
                        m_registerV[0xF] = 1;
                    else 
                        m_registerV[0xF] = 0;
                    m_registerV[regaddr_x] = add & 0x0FF;
                } 
                break;

                case 0x5 : {
                    if(m_registerV[regaddr_x] > m_registerV[regaddr_y])
                        m_registerV[0xF] = 1;
                    else 
                        m_registerV[0xF] = 0;
                    m_registerV[regaddr_x] = m_registerV[regaddr_x] - m_registerV[regaddr_y];
                }
                break;

                case 0x6 : {
                    if(m_registerV[regaddr_x] & 0x000F == 0x0001)
                        m_registerV[0xF] = 1;
                    else 
                        m_registerV[0xF] = 0;
                    m_registerV[regaddr_x] /= 0x02;
                }  
                break;

                case 0x7 : {
                    
                    if(m_registerV[regaddr_y] > m_registerV[regaddr_x])
                        m_registerV[0xF] = 1;
                    else 
                        m_registerV[0xF] = 0;
                    m_registerV[regaddr_x] = m_registerV[regaddr_y] - m_registerV[regaddr_x];
                }
                break;

                case 0xE : {
                    if((m_registerV[regaddr_x] & 0xF000) >> 12 == 1)
                        m_registerV[0xF] = 1;
                    else 
                        m_registerV[regaddr_y] *= 0x02;
                }
                break;
            }

        }
        break;

        case 0x9 : {
            if(m_registerV[regaddr_x] != m_registerV[regaddr_y])
                m_programCounter += 2;
        }
        break;

        case 0xA : {
            m_registerI = addr;
        }
        break;

        case 0xB : {
            m_programCounter = m_registerV[0x0] + addr;
        } 
        break;

        case 0xC : {
            srand(time(0));
            m_registerV[regaddr_x] = (uint8_t)(rand()%0x100) & byte;
        }
        break;

        case 0xD : {
            m_registerV[0xF] = 0;
            for(int y = 0;y < bit;y++){
                uint8_t sprite = m_ram[y + m_registerI];
                int row = (m_registerV[regaddr_y] + y) % 32;
                for(int x = 0;x < 8;x++){
                    int col = (m_registerV[regaddr_x] + x)% 64;
                    uint8_t bit = (sprite & (0x80 >> x)) >> (7 - x);
                    uint32_t pixel = m_display[row * 64 + col];
                    if(bit == 1){
                        if(pixel == 0xFFFFFFFF){
                            m_registerV[0xF] = 1;
                        }
                        m_display[row * 64 + col] ^= 0xFFFFFFFF;
                    }
                }
            }
            m_drawFlag = true;
        } 
            
        break;

        case 0xE : {
            switch(byte){
                
                case 0x9E :{
                    if(m_key == m_registerV[register_addr])
                        m_programCounter += 2;
                }
                break;

                case 0xA1 :{
                    if(m_key != m_registerV[register_addr])
                        m_programCounter += 2;
                } 
                break;

            }
        }
            
        break;

        case 0xF : {
            
            switch(instruction & 0x00FF){

                case 0x07 : {
                    m_registerV[regaddr_x] = m_delayTimers;
                }
                break;

                case 0x0A : 

    
                break;

                case 0x15 : {
                    m_delayTimers = m_registerV[regaddr_x];
                }
                break;   

                case 0x18 : {
                    m_soundTimers = m_registerV[regaddr_x];
                } 
                break;

                case 0x1E : {
                    m_registerI += m_registerV[regaddr_x];
                } 
                break;

                case 0x29 : {
                    uint8_t val = m_registerV[register_addr];
                    for(int i = 0; i < 0x50; i ++)
                        if(m_ram[i] == val){
                            m_registerI = i;
                            break;
                        }
                }
                break;

                case 0x33 : {
                    uint8_t ones = (m_registerV[register_addr] % 10);
                    uint8_t tens = ((m_registerV[register_addr] % 100) - ones)/10;
                    uint8_t hundereds = (m_registerV[register_addr] - ((tens * 10) + ones)) / 100;

                    m_ram[m_registerI] = hundereds;
                    m_ram[m_registerI + 1] = tens;
                    m_ram[m_registerI + 2] = ones;
                }
                break;

                case 0x55 : {
                    for(uint8_t i = 0;i <= regaddr_x;i++)
                        m_ram[m_registerI + i] = m_registerV[i];
                } 
                break;

                case 0x65 : {
                    for(uint8_t i = 0;i <= regaddr_x;i++)
                        m_registerV[i] = m_ram[m_registerI + i];
                } 
                break;            
            }
        } 
        break;
    }

    if(m_soundTimers > 0)
        m_soundTimers --;
    if(m_delayTimers > 0)
        m_delayTimers --;

    m_programCounter += 2;

}


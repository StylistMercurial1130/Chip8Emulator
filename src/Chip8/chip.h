#pragma once

#include <inttypes.h>
#include <iostream>

class Chip{

private : 

    uint8_t * m_registerV;
    uint8_t m_key;
    uint16_t m_registerI;
    uint8_t m_soundTimers,m_delayTimers;
    uint16_t m_programCounter;
    uint8_t m_stackPointer;
    uint16_t * m_stack;
    uint8_t * m_ram;
    uint32_t * m_display;
    bool m_drawFlag;

public : 

    Chip();
    ~Chip();
    void LoadRom(uint8_t * rom,int size);
    uint32_t * GetDisplay();
    bool GetDrawFlag();
    void disassemble();
    void ToggleDrawFlag();
    void ClearDisplay();

};


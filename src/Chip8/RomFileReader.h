#pragma once 

#include <iostream>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

class RomReader{

private :

    FILE * m_Reader;
    uint8_t * m_Rom;
    int m_Filesize; 

    int OpenFile(const char * fileName);
    void GetRom();
    int GetRomSize();


public : 

    RomReader(const char * fileName);
    uint8_t * ReturnRom();
    void CloseReader();
    void FreeRom();
    int ReturnRomSize();
    FILE * ReturnFileReader();
        
};
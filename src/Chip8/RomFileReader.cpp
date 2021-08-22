#include "RomFileReader.h"


void RomReader :: FreeRom(){ delete m_Rom; }
uint8_t * RomReader :: ReturnRom(){ return m_Rom; }
void RomReader :: CloseReader(){ fclose(m_Reader); }
int RomReader :: ReturnRomSize(){ return m_Filesize; }
FILE * RomReader :: ReturnFileReader(){ return m_Reader; }

int RomReader :: OpenFile(const char * fileName){

    m_Reader = fopen(fileName,"rb");
    if(m_Reader == NULL){
        return -1;
    } else 
        return 1;

}

int RomReader :: GetRomSize(){
    
    int start = ftell(m_Reader);
    fseek(m_Reader,0L,SEEK_END);
    int end = ftell(m_Reader);
    fseek(m_Reader,0L,SEEK_SET);

    int size = end - start;
    return size;

}

void RomReader :: GetRom(){

    if(m_Rom == NULL){
        m_Rom = new uint8_t[m_Filesize];
    }

    fread(m_Rom,1,m_Filesize,m_Reader);

}

RomReader :: RomReader(const char * fileName){ 
    
    m_Rom = NULL;

    if(OpenFile(fileName) < 0)
        std::cout<<"file could not opened !"<<std::endl;
    else {

        m_Filesize = GetRomSize();
        GetRom();

    }

}


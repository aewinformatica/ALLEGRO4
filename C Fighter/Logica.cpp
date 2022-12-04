#include "Logica.h"

Logica::Logica()
{
                Tempo = 99;
               
                
}

void Logica::contaTempo()
{
     
     if(Tempo > 0){
     Tempo--;
     }
};

void Logica::desenhar(BITMAP *Buffer){
                DATAFILE  *datFontes = NULL;
                datFontes = load_datafile("fontes.dat");
                           
     textprintf_centre_ex(Buffer, font, SCREEN_W/2, 0, makecol(54,54,54),-1,"Tempo:");
     textprintf_centre_ex(Buffer, (FONT *)datFontes[0].dat, SCREEN_W/2, 10, makecol(54,54,54),-1,"%d",Tempo);

}

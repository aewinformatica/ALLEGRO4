#ifndef CHAO_H
#define CHAO_H

// Arquivo: chao.h
// Data: 29/08/2007 

class Chao {

public:
   Chao();
   ~Chao();
   
   // verifica se colide com as coordenadas passadas
   bool colide( int, int, int );
   
   void setx( int ); // seta a posicao x
   void sety( int ); // seta a posicao y
   void set_tamanho( int ); // seta o tamanho do objeto
   
   int getx(); // retorna posicao x
   int gety(); // retorno posicao y
   int get_tamanho(); // retorna tamanho do objeto

private:

   int x;
   int y;
   int tamanho; 
   

};

#endif

#include "chao.h"

// Arquivo: chao.cpp
// Data: 29/08/2007 

Chao::Chao()
{
   setx(0); 
   sety(0);
   set_tamanho(0);
}

Chao::~Chao()
{
   setx(0); 
   sety(0);
   set_tamanho(0);
}

// verifica se colide com as coordenadas passadas
bool Chao::colide( int x, int y, int tamanho )
{
   if ( 
   this->x + this->tamanho > x && 
   this->x < x + tamanho && 
   this->y + this->tamanho > y && 
   this->y < y + tamanho
   )
   {
      return true;
   }
   
   return false;
}

// seta a posicao x
void Chao::setx( int vx )
{
   this->x = vx;
}

// seta a posicao y
void Chao::sety( int vy )
{
   this->y = vy;
}

// seta o tamanho do objeto
void Chao::set_tamanho( int vtamanho )
{
   this->tamanho = vtamanho;
} 


// retorna posicao x
int Chao::getx()
{
   return this->x;
} 

// retorno posicao y
int Chao::gety()
{
   return this->y;
} 

// retorna tamanho do objeto
int Chao::get_tamanho()
{
   return this->tamanho;
} 

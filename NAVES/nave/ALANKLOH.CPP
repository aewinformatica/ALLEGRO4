#include <stdio.h>
#include <conio.h>
#define ESQUERDA 75
#define DIREITA 77
#define BAIXO 80
#define CIMA 72  

int main(void)
{
   int posx, posy;
   char c;
   clrscr();
   posx=40;
   posy=12;
   gotoxy(posx,posy);
   c=getch();

   while (c==0)
   {
   c=getch();
                if (c==DIREITA)
                posx++;
                if (c==ESQUERDA)
                posx--;
                if (c==BAIXO)
                posy++;
                if (c==CIMA)
                posy--;
                if (posx>80)
                posx=1;
                if (posx<1)
                posx=80;
                if (posy>25)
                posy=1;
                if (posy<1)
                posy=25;
                gotoxy(posx,posy);
                c=getch();
   }
   return 0;
}
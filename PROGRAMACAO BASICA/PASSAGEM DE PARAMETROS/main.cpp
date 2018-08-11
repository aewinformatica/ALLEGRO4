#include <stdio.h>
#include <stdlib.h>

/* prototipo */
void Troca(int *px, int *py);

int main () 
{
   int a, b, c;

   printf ("Digite 3 numeros inteiros: ");
   scanf("%d %d %d", &a, &b, &c);
   if (a > b) Troca(&a, &b);
   printf("Em ordem crescente: %d\n", a);
   if (b > c) Troca(&b, &c);
   if (a > b) Troca(&a, &b);
   printf("Em ordem crescente: %d %d %d\n", a, b, c);

   system("pause");
   return 0;
}
void Troca(int *px, int *py)
/* Troca os valores das variaveis apontadas por px e py. */
{
   int n;

   n= *py;
   *py= *px;
   *px= n;
}

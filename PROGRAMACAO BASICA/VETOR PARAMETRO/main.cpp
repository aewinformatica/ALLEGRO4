#include <stdio.h>
#include <stdlib.h>

void inicia(int v[]) {
    for (int i = 0; i < sizeof(v); i++) {
        v[i] = 1 + rand() % 20;
    }
}

void mostra(int v[]) {
	int  n = (int) sizeof(v);
	printf("%d\n", n);
	
    for (int i = 0; i < sizeof(v); i++) {
    	printf("%d\n", n);
        printf("%d\n", v[i]);
    }
}

int main() {
    int vetor[8];
    // inicia(vetor);
    mostra(vetor);
    system("PAUSE");
}

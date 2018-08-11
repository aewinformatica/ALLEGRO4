#include <iostream>
using namespace std;

int main()
{
    int i;
    /*Quando o símbolo & é utilizado ao definirmos uma variável esta, em vez de ser uma variável normal,
     irá passar a ser uma referência para outra variável, ou seja, irá ser como um "nickname" (alcunha) para esta:
     Quando declaramos uma referência temos que atribuir logo a variável que queremos referenciar:*/
    int &i_ref = i;

    i = 20;

    cout << "Variavel i: " << i << endl;
    cout << "Referencia para i: " << i << endl;

    i_ref = 10;

    cout << "\nVariavel i: " << i << endl;
    cout << "Referencia para i: " << i << endl;
   /*Como podem ver pelo programa tanto i como i_ref possuem os mesmos valores e, se alterarmos o valor a uma delas,
    a outra irá necessariamente ser alterada para o mesmo valor. Como já devem ter percebido não há muitas vantagens 
    na utilização de referências neste caso, mas se as utilizarmos como argumentos para funções, verão a sua verdadeira
     utilidade:
    */
    
    /*se quiserem guardar o endereço de uma variável terão que o guardar através de um apontador*/
    int x = 10;
   // int v = &x;   //errado
    int *ponteiro= &x;   //correcto
    
    cout << "Variavel x:" << endl;
    cout << "\nValor de x: " << x << endl;
    cout << "Endereco de x: " << &x << endl;
    
    
    cout << "\n\nVariavel ponteiro:" << endl;
    cout << "\nValor de ponteiro: " << ponteiro << endl;
    cout << "Endereco de ponteiro: " << &ponteiro << endl;
    /*(o endereço de ponteiro. Reparem que está 4 "posições" antes do que X;
     isto acontece porque um int ocupa 4 bytes)*/
    cout << "Valor da variavel apontada por ponteiro (x): " << *ponteiro << endl;
    
    x += 10;

    cout << "\nValor de X: " << x << endl;
    cout << "Valor apontado por pointer: " << *ponteiro << endl;
    //valor da variável apontada pelo apontador temos que utilizar o operador desreferência (*)
    //Nunca se esqueçam de usar o operador desreferência sempre que quiserem trabalhar com o valor
    *ponteiro += 10;

    cout << "\nValor de X: " << x << endl;
    cout << "Valor apontado por pointer: " << *ponteiro << endl;

    ++*ponteiro; //ou ++(*pointer) se quiserem

    cout << "\nValor de X: " << x << endl;
    cout << "Valor apontado por pointer: " << *ponteiro << endl;

    
    cin.get();
    return 0;
}

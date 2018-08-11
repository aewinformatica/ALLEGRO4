#include <iostream>
using namespace std;

int main()
{
    int i;
    /*Quando o s�mbolo & � utilizado ao definirmos uma vari�vel esta, em vez de ser uma vari�vel normal,
     ir� passar a ser uma refer�ncia para outra vari�vel, ou seja, ir� ser como um "nickname" (alcunha) para esta:
     Quando declaramos uma refer�ncia temos que atribuir logo a vari�vel que queremos referenciar:*/
    int &i_ref = i;

    i = 20;

    cout << "Variavel i: " << i << endl;
    cout << "Referencia para i: " << i << endl;

    i_ref = 10;

    cout << "\nVariavel i: " << i << endl;
    cout << "Referencia para i: " << i << endl;
   /*Como podem ver pelo programa tanto i como i_ref possuem os mesmos valores e, se alterarmos o valor a uma delas,
    a outra ir� necessariamente ser alterada para o mesmo valor. Como j� devem ter percebido n�o h� muitas vantagens 
    na utiliza��o de refer�ncias neste caso, mas se as utilizarmos como argumentos para fun��es, ver�o a sua verdadeira
     utilidade:
    */
    
    /*se quiserem guardar o endere�o de uma vari�vel ter�o que o guardar atrav�s de um apontador*/
    int x = 10;
   // int v = &x;   //errado
    int *ponteiro= &x;   //correcto
    
    cout << "Variavel x:" << endl;
    cout << "\nValor de x: " << x << endl;
    cout << "Endereco de x: " << &x << endl;
    
    
    cout << "\n\nVariavel ponteiro:" << endl;
    cout << "\nValor de ponteiro: " << ponteiro << endl;
    cout << "Endereco de ponteiro: " << &ponteiro << endl;
    /*(o endere�o de ponteiro. Reparem que est� 4 "posi��es" antes do que X;
     isto acontece porque um int ocupa 4 bytes)*/
    cout << "Valor da variavel apontada por ponteiro (x): " << *ponteiro << endl;
    
    x += 10;

    cout << "\nValor de X: " << x << endl;
    cout << "Valor apontado por pointer: " << *ponteiro << endl;
    //valor da vari�vel apontada pelo apontador temos que utilizar o operador desrefer�ncia (*)
    //Nunca se esque�am de usar o operador desrefer�ncia sempre que quiserem trabalhar com o valor
    *ponteiro += 10;

    cout << "\nValor de X: " << x << endl;
    cout << "Valor apontado por pointer: " << *ponteiro << endl;

    ++*ponteiro; //ou ++(*pointer) se quiserem

    cout << "\nValor de X: " << x << endl;
    cout << "Valor apontado por pointer: " << *ponteiro << endl;

    
    cin.get();
    return 0;
}

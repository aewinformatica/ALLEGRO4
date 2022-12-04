#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Personagem{
      public:
             Personagem(const char * filename);
             Personagem(const string &filename);
             void Carrega_se( const char * filename );
             void Define_nome(string &str);
             const string Pega_nome() const;
             void Imprime() const;
             string name;
             string sobre;
             int Health;
            string raca;
            string arma;
            int x,y;
      private:
              
              double speed;
              
              
    
    protected:
    
    int type;
	int shadow;
      };

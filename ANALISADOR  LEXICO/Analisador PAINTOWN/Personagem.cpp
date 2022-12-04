#include "Personagem.h"
#include "token.h"
#include "tokenreader.h"
#include "token_exception.h"
#include "load_exception.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;



Personagem::Personagem(const char * filename){
name = "";
Carrega_se(filename);
};
Personagem::Personagem(const string &filename){
name = "";
Carrega_se(filename.c_str());
};
void Personagem::Carrega_se(const char *filename){
     
     	// setInvincibility( 1000 );
	TokenReader tr( filename );

	Token * head = NULL;
	try{
		head = tr.readToken();
	} catch( const TokenException & ex ){
		cerr<< "Nao pode ler o Arquivi "<<filename<<" : "<<ex.getReason()<<endl;
		// delete head;
		throw LoadException("Nao pode Abrir o Arquivo Personagem");
	}
	string xls = "Load time for ";
	xls += filename;

	if ( *head != "character" ){
		cout<<filename<< " is not a character"<<endl;
		// delete head;
		throw LoadException("File is not a character");
	}
		Token * n = NULL;
	try{

		while ( head->hasTokens() ){
			*head >> n;
			
			if ( *n == "name" ){
				*n >> name;

                }
                
            if(*n == "sobre" ){
                      *n >> sobre;
                }
                
			 if(*n == "health" ){
                      int h;
                      *n >> h;
                      Health = h;
                      }
              if(*n == "raca")
                      {
                            
                            *n >> raca;
                      }
              if(*n == "arma"){
                    *n >> arma;
                    }
              if(*n == "posicao"){
                    *n >> x >> y;
                    }
              

        }
        }
    catch( const TokenException & tex ){
		cout<< "TokenException: " << tex.getReason() << endl;
		n->print(" ");
		cout<<"* Dumping character"<<endl;
		head->print("*");
		// delete head;

		throw LoadException("Error parsing character");
	} catch( const LoadException & lex ){
		throw lex;
	}

     }
void Personagem::Imprime() const{
	cout<<"Name: "<<name<<endl;
	cout<<"Health: "<<Health<<endl;
	cout<<"Speed: "<<speed<<endl;
	cout<<"Type: "<<type<<endl;
	cout<<"Shadow: "<<shadow<<endl;

}
void Personagem::Define_nome(string &str){
     name = str;
     }

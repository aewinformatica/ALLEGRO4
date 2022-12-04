#include <fstream>
#include <iostream>
#include <string.h>//strchr
#include <vector>

#include "token.h"
#include "token_exception.h"
#include "tokenreader.h"

using namespace std;

TokenReader::TokenReader( const char * file ){
	ifile.open( file );
	meu_Arquivo = string( file );
	ifile >> noskipws;
	 cout<<"Abrindo.. "<<file<<endl;
}
TokenReader::TokenReader( const string & file ){
	ifile.open( file.c_str() );
	meu_Arquivo = file;
	ifile >> noskipws;
}

TokenReader::~TokenReader(){
	ifile.close();

	for ( vector< Token * >::iterator it = meus_tokens.begin(); it != meus_tokens.end(); it++ ){
		delete *it;
	}
}

Token * TokenReader::readToken() throw( TokenException ){

	if ( !ifile ) throw TokenException("Arquivo nao encontrado");

	char open_paren = 'x';
	char n;
	string string_atual = "";
	bool entre_aspas = false;
	
    // se deve ignorar o caracter seguinte
	bool ignorar = false;
	
	while ( ifile.good() && open_paren != '(' ){
		ifile >> open_paren;
	}
    //cria um ponteiro do tipo token
	Token * token_atual = new Token();
	//carrega o arquivo a ser pesquisado
	token_atual->setFile( meu_Arquivo );
    
    //insere o token atual no vetor de tokens 
    meus_tokens.push_back( token_atual );
	
	// o ponteiro recebe o valor do token atual
	Token * primeiro = token_atual;
	
	//cria vetor de ponteiros token
    vector< Token * > pilha_token;
	//insere o  token atual a pilha
    pilha_token.push_back( token_atual );
    
    //enquanto o vetor pilha nao tiver vazia
	while ( !pilha_token.empty() ){
		//se o arquivo nao for aberto
        if ( !ifile ){
		
			//primeiro->print( " " );
			throw TokenException("Numero errado de parenteses");
		}
        //passa o valor pra variavel n
		ifile >> n;
		
		const char * alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./-_!";
		const char * nonalpha = " ;()#\"";
		
        // cout<<"Alpha char: "<<n<<endl;
        //verifica se deve ignorar 
        if (ignorar){ 
            switch (n){
                 //se encontrar o "\n"   
                case 'n' : {
                    string_atual += "\n";
                    break;
                }
                //padrao adiciona a proxima letra a string_atual
                default : {
                    string_atual += n;
                    break;
                }
            }
            //ignorar passa a ser falso
            ignorar = false;
            //volta para o inicio do loop
            continue;
        }
        //se encontra \\ ignorar 
		if ( n == '\\' ){
			ignorar = true;
			continue;
		}

		if ( entre_aspas ){
             
			if ( n == '"' ){
                //encontrou a aspas mas so e considerado dentro da aspas quando encontra o fecha aspas
				entre_aspas = false;
				//cria um ponteiro sub inserindo a string_atual na criacao de um token que verifica se tem espaco e coloca ele em minusculo
				Token * sub = new Token( string_atual, false );
				//tokem_atual passa a ser pai
				sub->setParent( token_atual );
				//token atual adiciona ao vetor de token o token sub
				token_atual->addToken( sub );
				//string atual e vazia
				string_atual = "";

			} else
			    //continua adicionar a string mais caracteres
				string_atual += n;

		}
        //agora se verdade   
        else {
             //se n for igual a ""
			
            if ( n == '"' )
				entre_aspas = true;
				
				
				//se encontrar um caracter alpha na string 
			if ( strchr( alpha, n ) != NULL ){
                 //adiciona a string atual
				string_atual += n;
				
			}
            //se a string nao for vazia e e o retorno do caracter nao voltar nulo 
            else if ( string_atual != "" && strchr( nonalpha, n ) != NULL ){

                 //cout<<"Fez novo token "<<string_atual<<endl;
                 //
                Token * sub = new Token( string_atual, false );
				sub->setParent( token_atual );
				token_atual->addToken( sub );
				string_atual = "";
			}
		}
		if ( n == '#' || n == ';' ){
			while ( n != '\n' && !ifile.eof() ){
				ifile >> n;
			}
			continue;
		} else if ( n == '(' ){
               
			Token * another = new Token();
			another->setParent( token_atual );
			
            token_atual->addToken( another );
			token_atual = another;
			
			pilha_token.push_back( token_atual );

		} else if ( n == ')' ){
			if ( pilha_token.empty() ){	
				throw TokenException("Pilha Vazia");
			}
			pilha_token.pop_back();
			token_atual = pilha_token.back();	
		}
	}
	
    primeiro->print("");
    //first->finalize();
	return primeiro;

}

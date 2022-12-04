#include <iostream>
#include <algorithm>
#include <allegro.h>
#include <string>
#include <vector>
#include "Personagem.h"
#include "funcs.h"
using namespace std;
/*
template <typename Tipo>
    Tipo somar (Tipo a, Tipo b)
    {
        return a + b;
    }
*/
struct playerInfo{
	
	
	string path;
	playerInfo( string path ):path( path ){
	}
};

typedef vector<playerInfo> PlayerVector;

	

PlayerVector players;
void carregar(string base);
void init();
static PlayerVector loadPlayers( const char * path ){
       
vector< string > files = Util::getFiles( Util::getDataPath() + "/" + path, "*" );

std::sort( files.begin(), files.end() );

for(vector< string >::iterator it = files.begin();it != files.end();it++){
            
string file = (*it) + "/" + (*it).substr( (*it).find_last_of( '/' ) + 1 ) + "*.txt";
		cout << "Checking " << file << endl;
            
            }
            }
void init();
int main()
{
   init();
    /*
    cout << "Int - 2+3 - " << somar (2, 3) << endl;
    cout << "Double - 1.53+3.14 - " << somar (1.53, 3.14) << endl;
    cout << "Char - '1'+'2' - " << somar ('1', '2') << endl;*/
//string base = "data";
//const vector <string> personagens = Util::getFiles( base + "/", "*.txt" );

	/*if ( personagens.size() == 0 ){
		return "no-files!!!";
    }
    */





Personagem  *ch;



  
PlayerVector players = loadPlayers( "players/" );  

ch = new Personagem("bruno.txt");
    //ch->Define_nome(nome_);
cout <<"Nome: " << ch->name; cout <<endl;
cout <<"Sobre Nome: " << ch->sobre; cout <<endl;
cout << "Health :" << ch->Health; cout <<endl;
cout << "Raca :" << ch->raca; cout <<endl;
cout << "arma :" << ch->arma; cout <<endl;
cout << "X :" << ch->x; cout <<endl;
cout << "Y :" << ch->y; cout <<endl;

    while(!key[KEY_ESC]){
                
}
  
    
    return 0;
}
END_OF_MAIN();
void init(){
    allegro_init();
    install_keyboard();
    install_mouse();
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0);
     }

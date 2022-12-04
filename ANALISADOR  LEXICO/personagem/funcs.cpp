#include "funcs.h"
#include <allegro.h>
#include <vector>
#include <string>

using namespace std;

static string dataPath = "data";

vector< string > Util::getFiles( string dataPath, string find ){
	struct al_ffblk info;
	vector< string > files;

	if ( al_findfirst( (dataPath + find).c_str(), &info, FA_ALL ) != 0 ){
		return files;
	}
	files.push_back( dataPath + string( info.name ) );
	while ( al_findnext( &info ) == 0 ){
		files.push_back( dataPath + string( info.name ) );
	}
	al_findclose( &info );

	return files;
}
std::string Util::getDataPath(){
            
            return  dataPath + "/";
            }


bool Util::exists( const string & file ){
	return ::exists( file.c_str() ) != 0;
}

#ifndef _funcs_h
#define _funcs_h

#include <stdlib.h>
#include <vector>
#include <string>

namespace Util{

std::vector< std::string > getFiles( std::string dataPath, std::string find );
std::string getDataPath();
bool exists(const std::string &file);

}

#endif

#include <list>
#include <string>
#include <exception>
#include "parse-cache.h"
#include "parser/all.h"
#include "ast/all.h"
#include "ast/extra.h"
#include "globals.h"
#include "util/file-system.h"
#include "util/system.h"

using namespace std;

namespace Mugen{

static int replaceSlash(int what){
    if (what == '/'){
        return '-';
    }

    if (what == '\\'){
        return '-';
    }

    return what;
}

const char * MUGEN_CACHE = "mugen-cache";

/* true if path1 has a newer modification time than path2 */
static bool newer(const Filesystem::AbsolutePath & path1, const Filesystem::AbsolutePath & path2){
    return System::getModificationTime(path1.path()) >= System::getModificationTime(path2.path());
}

static list<Ast::Section*> * loadCached(const string & path){
    string converted = path;
    std::transform(converted.begin(), converted.end(), converted.begin(), replaceSlash);
    Filesystem::AbsolutePath fullPath = Filesystem::userDirectory().join(Filesystem::RelativePath(MUGEN_CACHE)).join(Filesystem::RelativePath(converted));
    if (!newer(fullPath, Filesystem::AbsolutePath(path))){
        throw MugenException("File is old");
    }
    TokenReader reader;
    Global::debug(1, "mugen-parse-cache") << "Loading from cache " << fullPath.path() << endl;
    return Ast::AstParse::deserialize(reader.readTokenFromFile(fullPath.path().c_str()));
}

static void saveParse(const Filesystem::AbsolutePath & path, list<Ast::Section*> * parse){
    ofstream out(path.path().c_str());
    Token * serial = Ast::AstParse::serialize(parse);
    serial->toStringCompact(out);
    out << endl;
    out.close();
    delete serial;
}

static void saveCached(list<Ast::Section*> * parse, const string & path){
    string converted = path;
    std::transform(converted.begin(), converted.end(), converted.begin(), replaceSlash);
    Filesystem::AbsolutePath cache = Filesystem::userDirectory().join(Filesystem::RelativePath(MUGEN_CACHE));

    if (!System::isDirectory(cache.path())){
        /* like mkdir -p */
        System::makeAllDirectory(cache.path());
    }

    Filesystem::AbsolutePath fullPath = cache.join(Filesystem::RelativePath(converted));
    Global::debug(1, "mugen-parse-cache") << "Saving cache to " << fullPath.path() << endl;
    saveParse(fullPath, parse);
}

/* attempts to load from a file on disk. if the file doesn't exist then
 * parse it for real and save it to disk.
 */
list<Ast::Section*> * Parser::loadFile(const string & path){
    try{
        return loadCached(path);
    } catch (const TokenException & fail){
    } catch (const MugenException & e){
    }

    list<Ast::Section*> * out = doParse(path);
    try{
        saveCached(out, path);
    } catch (...){
        Global::debug(0) << "Failed to save cached file " << path << endl;
        /* failed for some reason */
    }

    return out;
}

static void destroySectionList(list<Ast::Section*> * sections){
    for (list<Ast::Section*>::iterator section_it = sections->begin(); section_it != sections->end(); section_it++){
        delete (*section_it);
    }
    delete sections;
}

void Parser::destroy(){
    for (map<const string, list<Ast::Section*>* >::iterator it = cache.begin(); it != cache.end(); it++){
        list<Ast::Section*> * sections = (*it).second;
        if (sections != NULL){
            destroySectionList(sections);
        }
    }

    cache.clear();
}

Parser::Parser(){
}

Parser::~Parser(){
    destroy();
}

static list<Ast::Section*> * copy(list<Ast::Section*> * input){
    list<Ast::Section*> * output = new list<Ast::Section*>();

    for (list<Ast::Section*>::iterator it = input->begin(); it != input->end(); it++){
        output->push_back((Ast::Section*) (*it)->copy());
    }

    return output;
}

/* check for an in-memory copy of the parse. if it doesnt exist
 * then either load it from disk or parse it.
 * returns a new copy of the AST so you must delete it later.
 */
list<Ast::Section*> * Parser::parse(const std::string & path){
    if (cache[path] == NULL){
        cache[path] = loadFile(path);
    }

    return copy(cache[path]);
}

CmdCache::CmdCache(){
}

CmdCache::~CmdCache(){
}
    
AirCache::AirCache(){
}

AirCache::~AirCache(){
}

DefCache::DefCache(){
}

DefCache::~DefCache(){
}

static list<Ast::Section*> * reallyParseCmd(const std::string & path){
    return (list<Ast::Section*>*) Cmd::parse(path);
}

static list<Ast::Section*> * reallyParseAir(const std::string & path){
    return (list<Ast::Section*>*) Air::parse(path);
}

static list<Ast::Section*> * reallyParseDef(const std::string & path){
    return (list<Ast::Section*>*) Def::parse(path);
}

list<Ast::Section*> * CmdCache::doParse(const std::string & path){
    return reallyParseCmd(path);
}

list<Ast::Section*> * AirCache::doParse(const std::string & path){
    return reallyParseAir(path);
}

list<Ast::Section*> * DefCache::doParse(const std::string & path){
    return reallyParseDef(path);
}

ParseCache * ParseCache::cache = NULL;

list<Ast::Section*> * ParseCache::parseCmd(const string & path){
    if (cache == NULL){
        return reallyParseCmd(path);
    }
    return cache->doParseCmd(path);
}
    
list<Ast::Section*> * ParseCache::parseAir(const std::string & path){
    if (cache == NULL){
        return reallyParseAir(path);
    }
    return cache->doParseAir(path);
}

list<Ast::Section*> * ParseCache::parseDef(const std::string & path){
    if (cache == NULL){
        return reallyParseDef(path);
    }
    return cache->doParseDef(path);
}

void ParseCache::destroy(){
    if (cache){
        cache->destroyCache();
    }
}

ParseCache::ParseCache(){
    if (cache != NULL){
        Global::debug(0) << "Only one parse cache is allowed" << endl;
        throw exception();
    }
    cache = this;
}

void ParseCache::destroyCache(){
    cmdCache.destroy();
    airCache.destroy();
    defCache.destroy();
}

ParseCache::~ParseCache(){
    cache = NULL;
}

list<Ast::Section*> * ParseCache::doParseCmd(const std::string & path){
    return cmdCache.parse(path);
}
    
list<Ast::Section*> * ParseCache::doParseAir(const std::string & path){
    return airCache.parse(path);
}

list<Ast::Section*> * ParseCache::doParseDef(const std::string & path){
    return defCache.parse(path);
}

}

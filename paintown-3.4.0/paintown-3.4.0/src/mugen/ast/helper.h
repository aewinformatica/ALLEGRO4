#ifndef _paintown_ast_helper_h_
#define _paintown_ast_helper_h_

#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include "Value.h"
#include "util/funcs.h"
#include "util/token.h"

namespace Ast{

class Helper: public Value {
public:
    Helper(const std::string & name, Value * original):
        name(name),
        original(original){
        }

    static int lowerCase( int c ){
        return tolower(c);
    }
    
    virtual Element * copy() const {
        return new Helper(name, (Value*) original->copy());
    }

    static std::string downcase(std::string str){
        std::transform(str.begin(), str.end(), str.begin(), lowerCase);
        return str;
    }

    virtual Value * getOriginal() const {
        return original;
    }
    
    virtual void walk(Walker & walker) const {
        walker.onHelper(*this);
    }

    Token * serialize() const {
        Token * token = new Token();
        *token << SERIAL_HELPER << name << original->serialize();
        return token;
    }

    static Helper * deserialize(const Token * token){
        std::string name;
        const Token * next;
        token->view() >> name >> next;
        return new Helper(name, Value::deserialize(next));
    }

    using Element::operator==;
    bool operator==(const Value & him) const {
        return him == *this;
    }

    bool operator==(const Helper & him) const {
        return name == him.name &&
               *original == *him.original;
    }

    bool operator==(const std::string & str) const {
        return downcase(name) == downcase(str);
    }
    
    virtual std::string getType() const {
        return "helper";
    }

    virtual std::string toString() const {
        std::ostringstream out;
        out << name << ", " << original->toString();
        return Util::trim(out.str());
    }
    
    /*
    virtual bool referenced(const void * value) const {
        if (value == this){
            return true;
        }
        
        for (std::list<std::string*>::const_iterator it = names.begin(); it != names.end(); it++){
            if (value == *it){
                return true;
            }
        }

        return false;
    }
    */
    
    virtual void mark(std::map<const void*, bool> & marks) const {
        marks[this] = true;
        original->mark(marks);
    }

    virtual ~Helper(){
        delete original;
    }

protected:
    std::string name;
    Value * original;
};

}

#endif

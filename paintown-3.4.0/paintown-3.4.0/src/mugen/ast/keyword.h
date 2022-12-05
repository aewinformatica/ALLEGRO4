#ifndef _paintown_ast_keyword_h_
#define _paintown_ast_keyword_h_

#include "Value.h"
#include <string>

namespace Ast{

class Keyword: public Value {
public:
    /* do not pass in a dynamically allocated string!!! */
    Keyword(std::string str):
    str(str){
    }

    using Value::operator>>;

    virtual const Value & operator>>(std::string & str) const {
        str = this->str;
        return *this;
    }
    
    virtual void walk(Walker & walker) const {
        walker.onKeyword(*this);
    }
    
    virtual Element * copy() const {
        return new Keyword(str);
    }
    
    using Element::operator==;
    bool operator==(const Value & him) const {
        return him == *this;
    }

    bool operator==(const Keyword & him) const {
        return str == him.str;
    }

    bool operator==(const std::string & str) const {
        return downcase(toString()) == downcase(str);
    }

    static int lowerCase( int c ){
        return tolower(c);
    }

    Token * serialize() const {
        Token * token = new Token();
        *token << SERIAL_KEYWORD << toString();
        return token;
    }

    static Keyword * deserialize(const Token * token){
        std::string name;
        token->view() >> name;
        return new Keyword(name);
    }

    static std::string downcase(std::string str){
        std::transform(str.begin(), str.end(), str.begin(), lowerCase);
        return str;
    }
    
    virtual std::string getType() const {
        return "string";
    }

    virtual std::string toString() const {
        return str;
    }
    
    virtual ~Keyword(){
        /* don't delete str */
    }

protected:
    std::string str;
};

}

#endif

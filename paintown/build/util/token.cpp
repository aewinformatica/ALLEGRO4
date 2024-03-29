#include "token.h"
#include "token_exception.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <sstream>

using namespace std;

Token::Token():parent( NULL ){
	name = "HEAD";
	num_token = 1;
}

Token::Token( string tok, bool parse ):num_token( 1 ),parent( NULL ){

	/* legacy code, not used much */
	if ( !parse ){
		name = tok;
		while ( name.find(' ' ) == 0 )
			name.erase( 0, 1 );
		lowerCase( name );
		return;
	}
	
}

void Token::print( const string & space ){
	cout<<space<<"Token: "<< getName() << endl;
	for ( signed int i = 0; i < numTokens(); i++ ){
		Token * x = getToken( i );
		x->print( space + " |--" );
	}
}

void Token::lowerCase( string & s ){
	for ( unsigned int q = 0; q < s.length(); q++ ){
		if ( s[q] >= 'A' && s[q] <= 'Z' )
			s[q] = s[q] - 'A' + 'a';
	}
}

Token * Token::readToken(){
	if ( num_token < tokens.size() ){
		return tokens[ num_token++ ];
	}
	return NULL;
}
	
bool Token::hasTokens(){
	return num_token < tokens.size();
}
	
Token * Token::getToken( unsigned int n ){
	int q = numTokens();
	if ( q == -1 ) return NULL;
	if ( n >= 0 && (signed int)n < q )
		return tokens[n+1];
	return NULL;
}
	
const string & Token::getName() const {
	if ( numTokens() != -1 ){
		return tokens[0]->_getName();
	}
	// cout<<"No tokens!!"<<endl;
	return name;
}

const Token * const Token::getParent() const {
	return this->parent;
}

const string Token::getLineage() const {
	if ( getParent() != NULL ){
		return getParent()->getLineage() + " -> " + getName();
	}

	return getName();
}

void Token::setFile( const string & s ){
	filename = s;
}

const string Token::getFileName() const {
	if ( parent ){
		return parent->getFileName();
	} else {
		return filename;
	}
}

bool Token::operator== ( const string & rhs ){
	return getName() == rhs;
}

bool Token::operator!= ( const string & rhs ){
	return !( *this == rhs );
}

Token & Token::operator>>( Token * & rhs ) throw( TokenException ){
	Token * x = readToken();
	if ( x == NULL ){
		throw TokenException( getFileName() + ": " + string("Tried to read a token from ") + this->getName() + string(" but there are no more elements") );
	}
	rhs = x;
	return *this;
}

Token & Token::operator>>( string & rhs ) throw( TokenException ){
	Token * l = readToken();
	if ( l == NULL ){
		throw TokenException( getFileName() + ":" + string("Tried to read a string from ") + this->getLineage() + string(" but there no more elements") );
	}
	rhs = l->getName();

	// rhs = getName();

	return *this;
}
	
Token & Token::operator>>( int & rhs ) throw( TokenException ){
	Token * l = readToken();
	if ( l == NULL ){
		throw TokenException( getFileName() + ": " + string("Tried to read an int from ") + this->getLineage() + string(" but there are no more elements") );
	}
	istringstream is ( l->getName() );
	is >> rhs;
	return *this;
}

Token & Token::operator>>( double & rhs ) throw( TokenException ){
	Token * l = readToken();
	if ( l == NULL ){
		throw TokenException( getFileName() + ": " + string("Tried to read a double from ") + this->getLineage() + string(" but there no more elements") );
	}
	istringstream is ( l->getName() );
	is >> rhs;
	return *this;
}

Token & Token::operator>>( bool & rhs ) throw( TokenException ){
	Token * l = readToken();
	if ( l == NULL ){
		throw TokenException( getFileName() + ": " + string("Tried to read a bool from ") + this->getLineage() + string(" but there no more elements") );
	}
	istringstream is ( l->getName() );
	is >> rhs;
	return *this;
}

void Token::addToken( Token * t ){
	tokens.push_back( t );
}

void Token::finalize(){
	for ( vector< Token * >::iterator it = tokens.begin(); it != tokens.end(); ){
		Token * t = *it;
		if ( t->getName().find('!') == 0 ){
			delete t;
			it = tokens.erase( it );
		} else {
			t->finalize();
			it++;
		}
	}
}

Token::~Token(){
	for ( vector< Token * >::iterator it = tokens.begin(); it != tokens.end(); it++ )
		delete *it;
}

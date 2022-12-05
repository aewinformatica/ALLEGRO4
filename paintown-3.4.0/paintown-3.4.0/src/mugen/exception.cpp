#include <string>
#include <sstream>
#include "exception.h"
	
MugenException::MugenException():
Exception::Base("", -1),
reason("unspecified"),
where("?"),
line(0){
}

MugenException::MugenException(const std::string & reason, const std::string & where, int line):
Exception::Base("", line),
reason(reason),
where(where),
line(line){
}
        
Exception::Base * MugenException::copy() const {
    return new MugenException(reason, where, line);
}
        
const std::string MugenException::getFullReason() const {
    std::ostringstream out;
    out << where << ":" << line << " " << reason;
    return out.str();
}
	
MugenException::~MugenException() throw() {
}

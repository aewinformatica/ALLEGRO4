#ifndef _paintown_regex_h
#define _paintown_regex_h

#include <string>

namespace Util{
    bool matchRegex(const std::string & str, const std::string & regex);
    std::string captureRegex(const std::string & str, const std::string & regex, int capture);
}

#endif

#include "font-info.h"
#include "util/file-system.h"
#include "util/font.h"
#include "configuration.h"
#include "exceptions/exception.h"

namespace Menu{

FontInfo::FontInfo(){
}

FontInfo::~FontInfo(){
}

static bool menuFontAvailable(){
    return Configuration::getMenuFont() != NULL;
}

/*
std::string FontInfo::getName() const {
    return font.path();
}
        
const FontInfo & FontInfo::get(const FontInfo & next) const {
    return *this;
}

const Filesystem::RelativePath FontInfo::getFont(const FontInfo & next) const {
    return this->font;
}

const int FontInfo::getWidth(const FontInfo & next) const {
    return this->width;
}

const int FontInfo::getHeight(const FontInfo & next) const{
    return this->height;
}
*/
    
const Font & DefaultFontInfo::get(const Font & next) const {
    return next;
}

/*
const Filesystem::RelativePath DefaultFontInfo::getFont(const FontInfo & next) const {
    return next.getFont(next);
}
*/

const FontInfo & DefaultFontInfo::get(const FontInfo & next) const {
    return next;
}

const Font & DefaultFontInfo::get() const {
    throw Exception::Base(__FILE__, __LINE__);
}
    
std::string DefaultFontInfo::getName() const {
    return "Default";
}

const bool DefaultFontInfo::empty() const {
    return false;
}

const int DefaultFontInfo::getWidth(const FontInfo & next) const {
    throw Exception::Base(__FILE__, __LINE__);
}

const int DefaultFontInfo::getHeight(const FontInfo & next) const {
    throw Exception::Base(__FILE__, __LINE__);
}

#if 0
RelativeFontInfo::RelativeFontInfo(){
}

RelativeFontInfo::RelativeFontInfo(const Filesystem::RelativePath & font, int width, int height):
FontInfo(font, width, height){
}

const Font & RelativeFontInfo::get() const {
    return Font::getFont(font, width, height);
}

const Font & RelativeFontInfo::get(const Font & next) const {
    // Check for override from system and return that instead otherwise continue
    /*
    if (menuFontAvailable()){
        return Configuration::getMenuFont()->get();
    }
    */
    return Font::getFont(font, width, height);
}

const bool RelativeFontInfo::empty() const {
    return font.path().empty();
}

AbsoluteFontInfo::AbsoluteFontInfo(const Filesystem::AbsolutePath & font, int width, int height):
myfont(font),
mwidth(width),
mheight(height){
}

const Font & AbsoluteFontInfo::get() const {
    return Font::getFont(myfont, mwidth, mheight);
}

const Font & AbsoluteFontInfo::get(const Font & next) const {
    return get();
}
    
std::string AbsoluteFontInfo::getName() const {
    return myfont.getFilename().path();
}
#endif

}

#include <sdlpp_ttf.h>

#include <stdexcept>

namespace SdlPP {

//------------------------------------------------------------------------------
Font::Font(const std::string &ttfFile, int fontSize)
{
    font_ = TTF_OpenFont(ttfFile.c_str(), fontSize);
    if (font_ == NULL)
        throw std::runtime_error("Cannot open TTF font");
}

Font::~Font()
{
    if (font_ != nullptr)
        TTF_CloseFont(font_);
}


}   // SdlPP namespace

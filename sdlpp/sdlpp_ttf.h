#ifndef SDLPP_TTF_H
#define SDLPP_TTF_H

#include <SDL_ttf.h>

#include <string>

namespace SdlPP {

class Font
{
public:
    Font(const std::string &ttfFile, int fontSize);
    Font(const Font &other) = delete;
    ~Font();
    
    Font& operator=(const Font &other) = delete;

    TTF_Font* handle() const {return font_;}

private:
    TTF_Font *font_{nullptr};
};


}   // SdlPP namespace

#endif  // SDLPP_TTF_H

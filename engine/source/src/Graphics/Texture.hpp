//
//  Texture.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 12.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <inttypes.h>

class Texture {
public:

    Texture();
    ~Texture();
    

    bool loadFromFile(std::string path, SDL_Renderer* renderer);
    bool loadFromRenderedText(const uint16_t* str, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font *font);
    bool loadFromRenderedText(const char* str, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font *font);

    void free();
    
    void setColor(uint8_t red, uint8_t green, uint8_t blue );
    void setBlendMode(SDL_BlendMode blending );
    void setAlpha(uint8_t alpha );
    
    //Image dimensions
    int mWidth;
    int mHeight;
    
    SDL_Rect drawingRect;
    
    // hardware texture
    SDL_Texture* mTexture;    
    
};

#endif /* Texture_hpp */

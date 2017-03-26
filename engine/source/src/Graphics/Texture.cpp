//
//  Texture.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 12.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "Texture.hpp"
#include <string>
#include <SDL2/SDL_image.h>
#include <locale>
#include <iostream>
#include <codecvt>


using namespace std;

Texture::Texture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

Texture::~Texture() {
    free();
}

bool Texture::loadFromFile( std::string path, SDL_Renderer* renderer) {
    //Get rid of preexisting texture
    free();
    
    SDL_Texture* newTexture = NULL;
    
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF));
        
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        
        if( newTexture == NULL ) {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    mTexture = newTexture;
    return mTexture != NULL;
}

bool Texture::loadFromRenderedText( const uint16_t* str, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font *font) {
    //Get rid of preexisting texture
    free();
    
    SDL_Surface* textSurface = TTF_RenderUNICODE_Blended( font, str, textColor );
    if( textSurface == NULL ) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    } else {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( mTexture == NULL ) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        } else {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}

void Texture::free() {
    //Free texture if it exists
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::setColor( uint8_t red, uint8_t green, uint8_t blue ) {
    SDL_SetTextureColorMod( mTexture, red, green, blue );
    
}

void Texture::setBlendMode( SDL_BlendMode blending ) {
    SDL_SetTextureBlendMode( mTexture, blending );
    
}

void Texture::setAlpha( uint8_t alpha ) {
    SDL_SetTextureAlphaMod( mTexture, alpha );
    
}

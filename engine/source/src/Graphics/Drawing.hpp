//
//  SDLWideRect.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef SDLWideRect_hpp
#define SDLWideRect_hpp

#include <vector>
#include <SDL2/SDL.h>
#include "Texture.hpp"

namespace Drawing {
    void drawWideRectFill(SDL_Renderer* renderer, int x, int y, int w, int h, int width);
    void drawWideRect(SDL_Renderer* renderer, int x, int y, int w, int h, int lineWidth, int wideness);
    
    struct Size {
        int w, h;
    };
    
    class DrawingCtx {
    private:
        std::vector<SDL_Point> stack;
    public:
        TTF_Font* textFontHuge;
        TTF_Font* textFontBig;
        TTF_Font* textFont;

        int screenWidth;
        int screenHeight;

        SDL_Renderer* renderer;
        void pushStack();
        void popStack();
        void translate(const SDL_Point& pt);
        void translateX(int x);
        void translateY(int y);
        
        DrawingCtx(SDL_Renderer* renderer, int screenWidth, int screenHeight): renderer(renderer),
        screenWidth(screenWidth), screenHeight(screenHeight) {
            stack.push_back({0, 0});
        }
        
        // drawing
        void drawSDLRect(SDL_Rect r);
        void drawRect(Size s);
        
        void drawSDLWideRect(SDL_Rect r, int lineWidth, int wideness);
        void drawWideRect(Size s, int lineWidth, int wideness);
        
        void drawSDLWideRectFill(SDL_Rect r, int lineWidth);
        void drawWideRectFill(Size s, int lineWidth);
        
        void drawTextureAbsolute(const Texture& txt, SDL_Rect* clip = nullptr, double angle = 0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
        
        void drawTexture(const Texture& txt, SDL_Rect* clip = nullptr, double angle = 0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
        SDL_Point& getFromStack();
        
        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        

    };
}
#endif /* SDLWideRect_hpp */

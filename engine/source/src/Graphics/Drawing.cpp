//
//  SDLWideRect.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "Drawing.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>

using namespace std;

namespace Drawing {
    
    void drawRoundedRectFill(SDL_Renderer* renderer, int x, int y, int w, int h, int width) {
        for (int i = 0; i < width; i++) {
            roundedRectangleRGBA(renderer, x, y, x + w, y + h, 10, 0, 0, 0, 255);
            x += 1;
            y += 1;
            w -= 2;
            h -= 2;
        }
    }
    
    void drawWideRectFill(SDL_Renderer* renderer, int x, int y, int w, int h, int width) {
        
        SDL_Rect rect = {x, y, w, h};
        for (int i = 0; i < width; i++) {
            SDL_RenderDrawRect(renderer, &rect);
            rect.x += 1;
            rect.y += 1;
            rect.w -= 2;
            rect.h -= 2;
        }
        
    };

    
    void drawWideRect(SDL_Renderer* renderer, int x, int y, int w, int h, int lineWidth, int wideness) {
        auto diff = wideness - lineWidth + 1;
        auto change = (lineWidth - wideness - 1) * 2;
        drawWideRectFill(renderer, x, y, w, h, lineWidth);
        drawWideRectFill(renderer, x + diff, y + diff, w + change, h + change, lineWidth);
    }
    
    SDL_Point& DrawingCtx::getFromStack() {
        if (stack.empty()) {
            stack.push_back({0, 0});
        }
        return stack.back();
        
    }
    
    void DrawingCtx::pushStack() {
        auto current = getFromStack();
        stack.push_back(current);
    }
    
    void DrawingCtx::popStack() {
        stack.pop_back();
    }
    
    void DrawingCtx::translate(const SDL_Point &pt) {
        auto& current = getFromStack();
        current.x += pt.x;
        current.y += pt.y;
    }
    
    void DrawingCtx::translateX(int x) {
        auto& current = getFromStack();
        current.x += x;
    }
    
    void DrawingCtx::translateY(int y) {
        auto& current = getFromStack();
        current.y += y;
    }
    
    void DrawingCtx::drawSDLRect(SDL_Rect r) {
        auto& p = getFromStack();
        r.x += p.x;
        r.y += p.y;
        SDL_RenderDrawRect(renderer, &r);
        
    }
    void DrawingCtx::drawRect(Size s) {
        auto& p = getFromStack();
        SDL_Rect r = {p.x, p.y, s.w, s.h};
        SDL_RenderDrawRect(renderer, &r);
    }

    void DrawingCtx::drawSDLWideRect(SDL_Rect r, int lineWidth, int wideness) {
        auto& p = getFromStack();
        r.x += p.x;
        r.y += p.y;
        Drawing::drawWideRect(renderer, r.x, r.y, r.w, r.h, lineWidth, wideness);
    }
    
    void DrawingCtx::drawWideRect(Size s, int lineWidth, int wideness) {
        auto& p = getFromStack();
        Drawing::drawWideRect(renderer, p.x, p.y, s.w, s.h, lineWidth, wideness);
    }

    void DrawingCtx::drawSDLWideRectFill(SDL_Rect r, int lineWidth) {
        auto& p = getFromStack();
        r.x += p.x;
        r.y += p.y;
        Drawing::drawWideRectFill(renderer, r.x, r.y, r.w, r.h, lineWidth); 
    }
    
    void DrawingCtx::drawWideRectFill(Size s, int lineWidth) {
        auto& p = getFromStack();
        Drawing::drawWideRectFill(renderer, p.x, p.y, s.w, s.h, lineWidth);
      
    }
    
    void DrawingCtx::drawTextureAbsolute(const Texture& txt, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ) {
        
        SDL_SetRenderTarget(renderer, nullptr);

        //Render texture to screen
        SDL_RenderCopyEx( renderer, txt.mTexture, clip, &txt.drawingRect, angle, center, flip );
    }
    
    void DrawingCtx::drawTexture(const Texture& txt, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
        
        auto& current = getFromStack();
        
        SDL_Rect renderQuad = {
            current.x + txt.drawingRect.x,
            current.y + txt.drawingRect.y,
            txt.drawingRect.w,
            txt.drawingRect.h
        };

        SDL_SetRenderTarget(renderer, nullptr);
        
        //Render texture to screen
        SDL_RenderCopyEx( renderer, txt.mTexture, clip, &renderQuad, angle, center, flip );
    }
    
    void DrawingCtx::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        
    }

    
}

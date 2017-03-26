//
//  UIButton.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "UIButton.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>
UIButton::~UIButton() {
    delete txt;
    delete labelTexture;
}

UIButton::UIButton(const Drawing::DrawingCtx& ctx, const uint16_t* label, const SDL_Rect& rect):
rect(rect) {
    txt = new Texture();
    txt->loadFromFile("Resources/button.png", ctx.renderer);
    TTF_SizeUNICODE(ctx.textFont, label, &textRect.w, &textRect.h);
    
    labelTexture = new Texture();
    SDL_Color c {255, 255, 255, 255};
    labelTexture->loadFromRenderedText(label, c, ctx.renderer, ctx.textFont);
    calculateRect();
    
}
void UIButton::calculateRect() {
    auto diffX = (rect.w - textRect.w) / 2;
    textRect.x = diffX + rect.x;
    if (textRect.x + textRect.w > rect.w + rect.x) {
        textRect.x = rect.x + 8;
        textRect.w = rect.w - 16;
    }
    auto diffY = (rect.h - textRect.h) / 2;
    textRect.y = diffY + rect.y;
    if (textRect.y + textRect.h > rect.y + rect.h) {
        textRect.y = rect.y + 8;
        textRect.h = rect.h - 16;
    }
}
void UIButton::setRect(const SDL_Rect &rect) {
    this->rect = rect;
    calculateRect();
    
}

void UIButton::draw(Drawing::DrawingCtx& ctx) {

    SDL_Rect s = {0,0,rect.w, 18};

    if (state == UIButtonState::clicked) {
        s.y += 21;
    } else if (state == UIButtonState::hover) {
        s.y += 41;
    }
    
    SDL_RenderCopyEx(ctx.renderer, txt->mTexture, &s, &rect, 0, nullptr, SDL_FLIP_NONE);
    SDL_SetRenderTarget(ctx.renderer, nullptr);

    SDL_RenderCopyEx(ctx.renderer, labelTexture->mTexture, nullptr, &textRect, 0, nullptr, SDL_FLIP_NONE);
}

void UIButton::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        int x;
        int y;
        auto mouseState = SDL_GetMouseState(&x, &y);
        
        if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h) {
            state = (mouseState && SDL_BUTTON(SDL_BUTTON_LEFT)) ? UIButtonState::clicked : UIButtonState::hover;
        } else {
            state = UIButtonState::idle;
        }
    } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        if (state == UIButtonState::hover) {
            state = UIButtonState::clicked;
        }
    } else if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
        if (state == UIButtonState::clicked) {
            state = UIButtonState::hover;
            if (pushCallback) {
                pushCallback(*this);
            }
        }
    }
}

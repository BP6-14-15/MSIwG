//
//  UIButton.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef UIButton_hpp
#define UIButton_hpp

#include <string>
#include <functional>
#include <SDL2/SDL.h>
#include "UIObject.hpp"
#include "Texture.hpp"


enum class UIButtonState {
    idle, hover, clicked
};

class UIButton: public UIObject {
    SDL_Rect textRect;
    SDL_Rect rect;
    Texture* txt;
    Texture* labelTexture;
    void calculateRect();
    
public:
    UIButton(const UIButton& that) = delete;
    
    UIButtonState state = UIButtonState::idle;
    UIButton(const Drawing::DrawingCtx& ctx, const uint16_t* label, const SDL_Rect& rect);
    virtual ~UIButton();
    
    virtual void draw(Drawing::DrawingCtx& ctx) override;
    virtual void handleEvent(const SDL_Event& e);
    void setRect(const SDL_Rect& rect);
    
    
    std::function<void(UIButton&)> pushCallback = nullptr;

};
#endif /* UIButton_hpp */

//
//  MainMenuState.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef MainMenuState_hpp
#define MainMenuState_hpp

#include <stdio.h>
#include "GameState.hpp"
#include "UIButton.hpp"
#include "PlayState.hpp"

class GraphicsCtx;

class MainMenuState: public GameState {
    std::shared_ptr<GameCtx> gameCtx;
    
public:
    std::unique_ptr<UIButton> exitButton;
    std::unique_ptr<UIButton> playButton;
    
    MainMenuState(std::shared_ptr<GraphicsCtx> ctx, std::shared_ptr<GameCtx> gameCtx);
    
    virtual void draw() override;
    virtual void handleEvent(const SDL_Event& e) override;
    
    void exitBtnAction(UIButton& btn);
    void playBtnAction(UIButton& btn);
    void playStateDelegate(const PlayStateEvent& e);
};
#endif /* MainMenuState_hpp */

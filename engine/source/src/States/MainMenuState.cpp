//
//  MainMenuState.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "MainMenuState.hpp"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>


using namespace std;

MainMenuState::MainMenuState(std::shared_ptr<GraphicsCtx> ctx, std::shared_ptr<GameCtx> gameCtx): GameState(ctx), gameCtx(gameCtx) {
    SDL_Rect dummy = {100, 100, 150, 50};
    
    const auto playTxt = u"Play";
    playButton = std::make_unique<UIButton>(*(ctx->drawingCtx), (uint16_t*)playTxt, dummy);
    playButton->pushCallback = std::bind(&MainMenuState::playBtnAction, this, std::placeholders::_1);
    
    dummy.y += dummy.h + 8;
    const auto exitTxt = u"Exit";
    exitButton = std::make_unique<UIButton>(*(ctx->drawingCtx), (uint16_t*)exitTxt, dummy);
    exitButton->pushCallback = std::bind(&MainMenuState::exitBtnAction, this, std::placeholders::_1);
    
}

void MainMenuState::draw() {
    ctx->drawingCtx->pushStack();

    playButton->draw(*ctx->drawingCtx);
    exitButton->draw(*ctx->drawingCtx);
    ctx->drawingCtx->popStack();
}

void MainMenuState::handleEvent(const SDL_Event& e) {
    playButton->handleEvent(e);
    exitButton->handleEvent(e);
    
}

void MainMenuState::exitBtnAction(UIButton& btn) {
    ctx->isRunning = false;
    
}

void MainMenuState::playBtnAction(UIButton& btn) {
    ctx->stateMachine->registerState("play",
                                     make_shared<PlayState>(
                                                            ctx,
                                                            gameCtx,
                                                            std::bind(&MainMenuState::playStateDelegate, this, std::placeholders::_1)
                                                            )
                                     );
    ctx->stateMachine->setActive("play");
    btn.state = UIButtonState::idle; 
    
}

void MainMenuState::playStateDelegate(const PlayStateEvent &e) {
    switch (e) {
        case PlayStateEvent::didRequestExit:
            ctx->stateMachine->setActive("menu");
            ctx->stateMachine->unregister("play");
            break; 
    }
}


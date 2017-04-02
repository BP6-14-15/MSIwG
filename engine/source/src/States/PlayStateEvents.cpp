//
//  PlayStateEvents.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 23.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "PlayState.hpp"

using namespace std;

void PlayState::startBtnAction(UIButton& btn) {
    conf->initialBoardSprites.reserve(boardSprites.size());
    conf->initialPlayerSprites.reserve(playerSprites.size());
    
    for(const auto& boardSprite: boardSprites) {
        conf->initialBoardSprites.push_back(boardSprite->clone());
    }
    
    for(const auto& playerSprite: playerSprites) {
        conf->initialPlayerSprites.push_back(unique_ptr<Player>(new Player(*playerSprite.get())));
    }
    
    manualCleanup();
    
    nextPhase();
}

void PlayState::exit() {
    if (conf->currentPhase == PlayPhase::first) {
        manualCleanup();
    }
    PlayStateEvent pe = PlayStateEvent::didRequestExit;
    if (delegate) {
        delegate(pe);
    }
}

void PlayState::manualCleanup() {
    delete startButton;
    startButton = nullptr;
    delete reloadButton;
    reloadButton = nullptr;
}

void PlayState::reloadBtnAction(UIButton& btn) {
    initBoard(gameCtx->gameConf);
    
}

void PlayState::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) {
        exit();
        return;
    } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
        computeBoardPosition(gameCtx->gameConf);
    } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT && conf->currentPhase == PlayPhase::fifth) {
        exit();
        return;
    }
    
    if (startButton) {
        reloadButton->handleEvent(e);
        startButton->handleEvent(e);
    }
    
}

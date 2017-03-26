//
//  CPGame.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 15.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "CPGame.hpp"
#include "GameManager.hpp"
#include "GameContext.hpp"
#include "GraphicsContext.hpp"

using namespace std;
using namespace CPGame;
shared_ptr<GameCtx> gameCtx;



GameManager::GameManager(CPGame::PlayerControllerCallback firstPlayer,
                         CPGame::PlayerControllerCallback secondPlayer,
                         std::optional<CPGame::GameConfiguration> conf
                         ) {
    if (!conf) {
        conf = CPGame::GameConfiguration();
    }
    
    gameCtx = make_shared<GameCtx>(firstPlayer, secondPlayer, *conf);

}

GameManager::~GameManager() {
    
}

void GameManager::start() {
    auto graphicsCtx = make_shared<GraphicsCtx>(gameCtx);
    graphicsCtx->init(640, 480);
    graphicsCtx->enterMainLoop();
    
}

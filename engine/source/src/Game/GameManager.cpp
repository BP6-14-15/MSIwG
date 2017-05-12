//
//  CPGame.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 15.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "GameManager.hpp"
#include "GameContext.hpp"
#include "GraphicsContext.hpp"

using namespace std;
using namespace CPGame;
shared_ptr<GameCtx> gameCtx;


GameManager::GameManager(std::shared_ptr<GameRemotePlayer>  firstPlayer,
                         std::shared_ptr<GameRemotePlayer>  secondPlayer,
                         std::optional<GameConfiguration> conf
                         ) {
    if (!conf) {
        conf = GameConfiguration();
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



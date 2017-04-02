//
//  GameContext.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <optional>
#include <random>
#include "GameContext.hpp"
#include "GraphicsContext.hpp"

using namespace std;
using namespace CPGame;

bool rollDiceWithProbability(int prob, std::mt19937& generator) {
    std::uniform_int_distribution<> d(0, 100);
    return prob >= d(generator);
    
}

int rollProbabilityDice(std::mt19937& generator) {
    std::uniform_int_distribution<> d(0, 100);
    return d(generator);
    
}


GameCtx::GameCtx(CPGame::PlayerControllerCallback firstPlayer, CPGame::PlayerControllerCallback secondPlayer, CPGame::GameConfiguration gameConf)
:gameConf(gameConf) {
    if (gameConf.boardSize < 20) {
        throw GameCtxException(GameCtxExceptionType::incorrectBoardSize);
    }
    
    if (gameConf.nPolice <= 0) {
        throw GameCtxException(GameCtxExceptionType::incorrectPoliceAmount);
    }
    
    if (gameConf.nWalls <= 0) {
        throw GameCtxException(GameCtxExceptionType::incorrectWallsAmount);

    }
    
    if (gameConf.nGates <= 0) {
        throw GameCtxException(GameCtxExceptionType::incorrectGatesAmount);
        
    }
    
    random_device device;
    randomGenerator = mt19937(device());
    updateManager.criminalUpdate = firstPlayer;
    updateManager.policemanUpdate = secondPlayer;
}


GameCtx::~GameCtx() {
    
}

void GameUpdateManager::switchPlayers() {
    auto criminal = criminalUpdate;
    criminalUpdate = policemanUpdate;
    policemanUpdate = criminal;
}

template<class T>
chrono::milliseconds waitForFuture(future<T>& fObj, const std::chrono::time_point<std::chrono::high_resolution_clock>& start, const chrono::milliseconds& limit) {
    auto end = chrono::high_resolution_clock::now();
    
    chrono::milliseconds m(33);
    while ((end - start + m) < limit) {
        auto state = fObj.wait_for(m--);
        
        if (state == std::__1::future_status::ready) {
            return chrono::duration_cast<chrono::milliseconds>(limit - (end - start));
        }
        end = chrono::high_resolution_clock::now();
    }
    return limit;
    
}

tuple<
    shared_ptr<CPGame::BoardPlayerUpdateResult>,
    shared_ptr<CPGame::BoardPlayerUpdateResult>
>
GameUpdateManager::makeRequest(
                               const Board& board,
                               const CPGame::BoardPlayerUpdateRequest& policemanReq,
                               const CPGame::BoardPlayerUpdateRequest& criminalReq
) {

    auto policemanUpdatePromise = make_shared<CPGame::Promise<BoardPlayerUpdateResult>>();
    shared_ptr<BoardPlayerUpdateResult> policemanUpdateResult = nullptr;
    

    auto criminalUpdatePromise = make_shared<CPGame::Promise<BoardPlayerUpdateResult>>();

    shared_ptr<BoardPlayerUpdateResult> criminalUpdateResult = nullptr;

    auto start = chrono::high_resolution_clock::now();
    assert(policemanUpdate);
    assert(criminalUpdate);
    
    
    shared_ptr<const Board> sharedState = make_shared<const Board>(board);
    
    thread policemanThread(policemanUpdate, sharedState, policemanReq, policemanUpdatePromise);
    thread criminalThread(criminalUpdate, sharedState, criminalReq, criminalUpdatePromise);
    policemanThread.detach();
    criminalThread.detach();
    this_thread::sleep_until(start + chrono::milliseconds(500));
    
    criminalUpdateResult = criminalUpdatePromise->getValue();
    policemanUpdateResult = policemanUpdatePromise->getValue();

    return make_tuple(policemanUpdateResult, criminalUpdateResult);
    
}


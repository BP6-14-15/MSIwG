//
//  GameContext.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <random>
#include <iostream>
#include <unistd.h>
#include "GameContext.hpp"
#include "GraphicsContext.hpp"
#include "Utils.hpp"

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


GameCtx::GameCtx(std::shared_ptr<GameRemotePlayer>  firstPlayer, std::shared_ptr<GameRemotePlayer>  secondPlayer, const GameConfiguration& gameConf)
: gameConf(gameConf), updateManager(GameUpdateManager(firstPlayer, secondPlayer)) {
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
    
    if (gameConf.customSeed) {
        initialSeed = *gameConf.customSeed;
    } else {
        random_device device;
        initialSeed = device();

    }
    randomGenerator = mt19937(initialSeed);
}


GameCtx::~GameCtx() {
    
}

void GameCtx::resetGenerator() {
    randomGenerator = mt19937(initialSeed);
    
}

void GameUpdateManager::setActivePlayersForRound(GameRound round) {
    switch (round) {
    case GameRound::first:
            activeCriminalUpdate = firstPlayerUpdate;
            activePoliceUpdate = secondPlayerUpdate;
            break;
    case GameRound::second:
            activeCriminalUpdate = secondPlayerUpdate;
            activePoliceUpdate = firstPlayerUpdate;
            break;
    }
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

GameUpdateManager::GameUpdateManager(std::shared_ptr<GameRemotePlayer> firstPlayerUpdate, std::shared_ptr<GameRemotePlayer> secondPlayerUpdate)
: firstPlayerUpdate(firstPlayerUpdate), secondPlayerUpdate(secondPlayerUpdate) {
    this->setActivePlayersForRound(GameRound::first);
    
    _clientLogDescriptor = open("engine.log", O_WRONLY | O_CREAT, 0755);
    
    if (_clientLogDescriptor == -1) {
        std::cerr << "Fatal error. Couldn't open engine.log" << std::endl;
        exit(1);
    }
    lseek(_clientLogDescriptor, SEEK_END, 0);
}

GameUpdateManager::~GameUpdateManager() {
    if (_clientLogDescriptor != -1) {
        close(_clientLogDescriptor);

    }
}

tuple<
    shared_ptr<CPGame::BoardPlayerUpdateResult>,
    shared_ptr<CPGame::BoardPlayerUpdateResult>
>
GameUpdateManager::makeRequest(
                               const Board& board,
                               const CPGame::BoardPlayerUpdateRequest& policemanReq,
                               const CPGame::BoardPlayerUpdateRequest& criminalReq,
                               bool lockSTDOut
) {

    auto policemanUpdatePromise = make_shared<CPGame::Promise<BoardPlayerUpdateResult>>();
    shared_ptr<BoardPlayerUpdateResult> policemanUpdateResult = nullptr;
    

    auto criminalUpdatePromise = make_shared<CPGame::Promise<BoardPlayerUpdateResult>>();

    shared_ptr<BoardPlayerUpdateResult> criminalUpdateResult = nullptr;

    assert(activePoliceUpdate);
    assert(activeCriminalUpdate);
    
    shared_ptr<const Board> sharedState = make_shared<const Board>(board);
    
    cout << endl;
    cout << "Police signature: " << activePoliceUpdate->signatureCallback() << endl;
    cout << "Criminal signature: " << activeCriminalUpdate->signatureCallback() << endl;

    int currentOutDescriptor = -1;
    if (lockSTDOut) {
        currentOutDescriptor = dup(STDOUT_FILENO);
        
        if (currentOutDescriptor == -1) {
            cerr << "Warning: Couldn't duplicate STDOUT" << endl;
        } else {
            int dup2Res = dup2(_clientLogDescriptor, STDOUT_FILENO);
            if (dup2Res == -1) {
                cerr << "Warning: Couldn't redirect STDOUT" << endl;
                close(currentOutDescriptor);
                currentOutDescriptor = -1;
            } else {
                cout << "\n" << currentDateAndTime() << endl;
            }
        }
        
        
    }
    
    thread policemanThread(activePoliceUpdate->controlCallback, sharedState, policemanReq, policemanUpdatePromise);
    thread criminalThread(activeCriminalUpdate->controlCallback, sharedState, criminalReq, criminalUpdatePromise);
    
    auto start = chrono::high_resolution_clock::now();

    policemanThread.detach();
    criminalThread.detach();
    
    this_thread::sleep_until(start + chrono::milliseconds(500));
    
    if (lockSTDOut && currentOutDescriptor != -1) {
        dup2(currentOutDescriptor, STDOUT_FILENO);
        
        close(currentOutDescriptor);
        
    }
    
    criminalUpdateResult = criminalUpdatePromise->getValue();
    policemanUpdateResult = policemanUpdatePromise->getValue();
    
    return make_tuple(policemanUpdateResult, criminalUpdateResult);
    
}


//
//  GameManager.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 02.04.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef GameManager_h
#define GameManager_h

#include <optional>
#include <inttypes.h>
#include "CPGame.hpp"

struct GameConfiguration {
    
    // 20 is minimum
    int boardSize = 20;
    int nPolice = 5; // > 0
    int nGates = 2; // > 1
    int wGates = 2;
    int nWalls = 4; // > 1
    int lWalls = 4;
    
    // probability 0 - 100
    int pGM = 50; // probability of gate move
    int pWM = 75; // - // - wall - // -
    int pGDC = 1; // probabilty of gate direction change
    int pWDC = 5; // - // - wall - // -
    
    int clockLimit = 100; // T
    
    bool allowsPlayersSTDOut = false;
    bool printsMoveDirections = false;
    
    std::optional<uint_fast32_t> customSeed = std::nullopt;
    bool applyCustomSeedToDefaultClient = false;
    
    void somefcn();
    
};

class GameRemotePlayer {
public:
    CPGame::PlayerControllerCallback controlCallback;
    CPGame::PlayerSignatureCallback signatureCallback;
    
    GameRemotePlayer(CPGame::PlayerControllerCallback control, CPGame::PlayerSignatureCallback signature)
    : controlCallback(control), signatureCallback(signature) {
        
    }
    GameRemotePlayer(const GameRemotePlayer&) = delete;
    
};

class GameManager {
public:
    GameManager(std::shared_ptr<GameRemotePlayer>  firstPlayer,
                std::shared_ptr<GameRemotePlayer>  secondPlayer,
                std::optional<GameConfiguration> conf
                );
    ~GameManager();
    
    void start();
    
};

#endif /* GameManager_h */

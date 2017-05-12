//
//  GameContext.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef GameContext_hpp
#define GameContext_hpp

#include <random>
#include <future>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include "GameManager.hpp"
#include "GameCtxException.hpp"
#include "Board.hpp"


bool rollDiceWithProbability(int prob, std::mt19937& generator);

int rollProbabilityDice(std::mt19937& generator);

enum class GameRound {
    first, second
};

class GameUpdateManager {
    friend class GameCtx;
    
    GameUpdateManager(std::shared_ptr<GameRemotePlayer> firstPlayerUpdate, std::shared_ptr<GameRemotePlayer> secondPlayerUpdate);
    
public:
    GameUpdateManager(const GameUpdateManager&) = delete;
    ~GameUpdateManager();
    
    int _devNullDescriptor;
    const std::shared_ptr<GameRemotePlayer> secondPlayerUpdate;
    const std::shared_ptr<GameRemotePlayer> firstPlayerUpdate;
    
    std::shared_ptr<GameRemotePlayer> activeCriminalUpdate;
    std::shared_ptr<GameRemotePlayer> activePoliceUpdate;
    
    std::tuple<std::shared_ptr<CPGame::BoardPlayerUpdateResult>,
    std::shared_ptr<CPGame::BoardPlayerUpdateResult>>
    makeRequest(const CPGame::Board&, const CPGame::BoardPlayerUpdateRequest& policemanReq,
                     const CPGame::BoardPlayerUpdateRequest& criminalReq, bool lockSTDOutput);
    
    void setActivePlayersForRound(GameRound round);
    
};

class ColisionCtx {
public:
    bool thiefWasCaught = false;
    bool thiefDidEscape = false;
    
    inline void reset() {
        thiefWasCaught = false;
        thiefDidEscape = false; 
    }
};

class GameCtx: public std::enable_shared_from_this<GameCtx> {    

public:
    std::mt19937 randomGenerator;
    uint_fast32_t initialSeed; 
    std::vector<std::vector<CPGame::BoardPosition>> stateCache;
    
    // stateCache indexes
    
    // <0, cacheGateIndex) : walls, <cacheGateIndex, cachePlayerIndex) - gates
    // <cachePlayerIndex, cacheCriminalIndex) - police
    int cacheGateIndex;
    // all none players sprites are first in stateCache
    int cachePlayerIndex;
    // criminal always last
    int cacheCriminalIndex;
    
    
    const GameConfiguration gameConf;
    GameUpdateManager updateManager;
    ColisionCtx colisionCtx;

    
    // firstPlayer starts as criminal 
    GameCtx(std::shared_ptr<GameRemotePlayer> firstPlayer, std::shared_ptr<GameRemotePlayer> secondPlayer, const GameConfiguration& gameConf);
    ~GameCtx();
    GameCtx(const GameCtx& cp) = delete;
    
    
    void resetGenerator(); 
    
    // [0 - 100]
    inline bool rollDiceWithProbability(int prob) {
        return ::rollDiceWithProbability(prob, randomGenerator);
        
    }
    
    inline int rollProbabilityDice() {
        return ::rollProbabilityDice(randomGenerator);
        
    }
    
    
};
#endif /* GameContext_hpp */

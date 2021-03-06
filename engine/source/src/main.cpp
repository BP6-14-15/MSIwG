//
//  main.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 09.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <random>
#include <iostream>
#include "GameManager.hpp"
#include "CommandLine.hpp"
#include "GameCtxException.hpp"

using namespace std;
using namespace CPGame;

mt19937 randomGenerator;

void updatePlayer(shared_ptr<const CPGame::Board> state,
                  CPGame::BoardPlayerUpdateRequest req,
                  shared_ptr<CPGame::Promise<CPGame::BoardPlayerUpdateResult>> promise
                  ) {
    std::cout << "hehe \n haha \n hihih" << std::endl;
    uniform_int_distribution<> d(1, 125);
    CPGame::BoardPlayerUpdateResult res;
    
    for(int i = 0; i < req.objectIndexes.size(); i++) {
        vector<BoardMoveDirection> dirs;
        
        for (int j = 0; j < req.numberOfMovesRequired; j++) {
            BoardMoveDirection dir;
            int roll = d(randomGenerator);
            
            if (roll < 25) {
                dir = BoardMoveDirection::left;
                
            } else if (roll < 50) {
                dir = BoardMoveDirection::top;
                
            } else if (roll < 75) {
                dir = BoardMoveDirection::right;
                
            } else if (roll < 100) {
                dir = BoardMoveDirection::bottom;
                
            } else {
                dir = BoardMoveDirection::none;
                
            }
            dirs.push_back(dir);
            
        }
        res.moveDirection.push_back(dirs);
        
    }
    promise->setValue(res);
    
}

const char* defaultFirstPlayerSignature(void) {
    return "default first player signature";
    
}


const char* defaultSecondPlayerSignature(void) {
    return "default second player signature";
    
}

int main(int argc, char* args[]) {
    PlayersSource src(
                      std::make_shared<GameRemotePlayer>(updatePlayer, defaultFirstPlayerSignature),
                      std::make_shared<GameRemotePlayer>(updatePlayer, defaultSecondPlayerSignature)
                      );

    GameConfiguration conf;
    parseArg(argc, args, conf, src);
    
    printf("Configuration: \n");
    printf("\tboard size: %d\n", conf.boardSize);
    printf("\tn police: %d\n", conf.nPolice);
    printf("\tn gates: %d\n", conf.nGates);
    printf("\tgates width: %d\n", conf.wGates);
    printf("\tn walls: %d\n", conf.nWalls);
    printf("\twalls length %d\n", conf.lWalls);
    printf("\tgate move probability: %.2f\n", double(conf.pGM) / 100);
    printf("\tgate direction change probability: %.2f\n", double(conf.pGDC) / 100);
    printf("\twall move probability: %.2f\n", double(conf.pWM) / 100);
    printf("\twall direction change probability: %.2f\n", double(conf.pWDC) / 100);
    printf("\tclock limit: %d\n", conf.clockLimit);
    
    if (conf.customSeed) {
        cout << "\tseed: " << *conf.customSeed << endl;
    }
    
    if (conf.customSeed && conf.applyCustomSeedToDefaultClient) {
    
        randomGenerator = mt19937(*conf.customSeed);
    } else {
        random_device r;
        randomGenerator = mt19937(r());
    }
    
    printf("\n");
    
    
    try {
        GameManager manager(src.player1, src.player2, conf);
        manager.start();

    } catch (GameCtxException e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    printf("\n");
    
    return 0;
}

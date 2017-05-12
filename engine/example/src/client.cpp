//
//  client.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 09.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <random>
#include <iostream>
#include "CPGame.hpp"

using namespace std;
using namespace CPGame;

mt19937 randomGenerator;


extern "C" {
    
    const char* gameEnginePlayerSignature(void) {
        return "Example signature";
        
    }
    
    
    void updatePlayer(shared_ptr<const Board> states,
                      BoardPlayerUpdateRequest req,
                      shared_ptr<Promise<BoardPlayerUpdateResult>> promise
                      ) {
        uniform_int_distribution<> d(1, 125);
        BoardPlayerUpdateResult res;
        
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
}


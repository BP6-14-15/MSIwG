//
//  CPGame.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 15.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef CPGame_hpp
#define CPGame_hpp

#include <vector>
#include <functional>
#include "Board.hpp"
#include "Promise.hpp"

namespace CPGame {
    
    enum class BoardObjectType {
        player, wall, gate
        
    };
    
    struct PlayerObjectData {
        PlayerType type;
        
    };
    
    union BoardObjectData {
        PlayerObjectData player;
        
    };
    
    struct BoardObject {
        std::vector<BoardPosition> coveredFields; // x, y - < 0, n + 1>
        BoardObjectType type;
        BoardObjectData data;
    };
    
    struct BoardState {
        std::vector<BoardObject> objects;
        
    };
    
    struct Board {
        std::vector<BoardState> stateHistory; // last is the actual
        int boardSize; // board { n x n }
        
    };
    
    
    struct BoardPlayerUpdateRequest {
        std::vector<int> objectIndexes;
        int numberOfMovesRequired = 5;
        
    };
    
    struct BoardPlayerUpdateResult {
        std::vector<std::vector<BoardMoveDirection>> moveDirection;
        
    };
    
    typedef std::function<void(std::shared_ptr<const Board>, BoardPlayerUpdateRequest, std::shared_ptr<Promise<BoardPlayerUpdateResult>> res)>PlayerControllerCallback;
    
}


#endif /* CPGame_hpp */

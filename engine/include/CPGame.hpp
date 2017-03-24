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
    
    union BoardData {
        PlayerObjectData player;
        
    };
    
    struct BoardObject {
        std::vector<BoardPosition> coveredFields; // x, y - < 0, n + 1>
        BoardObjectType type;
        BoardData data;
    };
    
    struct BoardState {
        std::vector<BoardObject> objects;
        int boardSize; // board { n x n }
    };
    
    struct BoardPlayerUpdateRequest {
        std::vector<int> objectIndexes;
        int numberOfMovesRequired = 5;
        
    };
    
    struct BoardPlayerUpdateResult {
        std::vector<std::vector<BoardMoveDirection>> moveDirection;
        
    };
    
    enum class RequestType {
        policeman, criminal
        
    };
    
    typedef std::function<void(std::shared_ptr<const std::vector<CPGame::BoardState>>, BoardPlayerUpdateRequest, std::shared_ptr<Promise<BoardPlayerUpdateResult>> res)>PlayerControllerCallback;
    
    class GameManager {
    public:
        GameManager(CPGame::PlayerControllerCallback firstPlayer, CPGame::PlayerControllerCallback secondPlayer);
        ~GameManager();
        
        void start();
    };
}


#endif /* CPGame_hpp */

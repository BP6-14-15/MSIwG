//
//  Board.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 12.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <stdio.h>

namespace CPGame {
    
    struct BoardPosition {
        int x,y;
        
        bool operator==(const BoardPosition& a) const {
            return (x == a.x && y == a.y);
            
        }
    };

    enum class PlayerType {
        police, criminal
    };

    enum class BoardMoveDirection {
        left, top, right, bottom, none
    };
}

#endif /* Board_hpp */

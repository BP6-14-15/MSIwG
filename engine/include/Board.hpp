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
#include <vector>
#include <ostream>

namespace CPGame {
    
    struct BoardPosition {
        int x, y;
        
        bool operator==(const BoardPosition& a) const {
            return (x == a.x && y == a.y);
            
        }
        
        bool operator<(const BoardPosition& a) const {
            return (x + y < a.x + a.y);
            
        }
        
    };
    

    enum class PlayerType {
        police, criminal
        
    };

    enum class BoardMoveDirection {
        left, top, right, bottom, none
        
    };
}

inline std::ostream& operator<<(std::ostream& stream, const CPGame::BoardPosition& pos) {
    stream << "(" << pos.x << ", " << pos.y << ")";
    return stream;
    
}


inline std::ostream& operator<<(std::ostream& stream, const CPGame::BoardMoveDirection& pos) {
    std::string txt;
    
    switch (pos) {
        case CPGame::BoardMoveDirection::bottom:
            txt = " \\/ ";
            break;
        case CPGame::BoardMoveDirection::top:
            txt = " /\\ ";
            break;
        case CPGame::BoardMoveDirection::left:
            txt = " <- ";
            break;
        case CPGame::BoardMoveDirection::right:
            txt = " -> ";
            break;
        case CPGame::BoardMoveDirection::none:
            txt = " -- ";
            break;
    }
    
    stream << txt;
    return stream;
    
}


#endif /* Board_hpp */

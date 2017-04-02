//
//  BoardSprite.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 11.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef BoardSprite_hpp
#define BoardSprite_hpp

#include <stdio.h>
#include <assert.h>
#include "Drawing.hpp"
#include "GameContext.hpp"


class PlayState; 

// spreading into direction - drawing
enum class BoardDirection {
    right, bottom
};


class BoardSprite {
protected:
    std::vector<CPGame::BoardPosition> coveredFields;

public:
    CPGame::BoardPosition pos;
    BoardDirection dir;
    int length;
    
    BoardSprite(CPGame::BoardPosition pos, BoardDirection dir, int length): pos(pos), dir(dir), length(length) {
        
    }
    
    virtual ~BoardSprite() {
        
    }
    
    virtual void draw(Drawing::DrawingCtx& ctx, int squareSize);
    virtual const std::vector<CPGame::BoardPosition>& update(GameCtx& ctx, const PlayState& playState) = 0;
    virtual const std::vector<CPGame::BoardPosition>& getCoveredFields(GameCtx& ctx) = 0;
    
    
    virtual std::unique_ptr<BoardSprite> clone() const = 0;
    
    
};
#endif /* BoardSprite_hpp */

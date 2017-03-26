//
//  Wall.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 11.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <stdio.h>
#include <assert.h>
#include "BoardSprite.hpp"

class Wall: public BoardSprite {
    CPGame::BoardMoveDirection moveDir;

public:
    int dirChangeProbabiltiy;
    int moveProbability;
    
    Wall(CPGame::BoardPosition pos, BoardDirection dir, CPGame::BoardMoveDirection moveDir, int length, const GameCtx& ctx);
    
    virtual void draw(Drawing::DrawingCtx& ctx, int squareSize) override;
    virtual const std::vector<CPGame::BoardPosition>&  update(GameCtx& ctx) override;
    virtual const std::vector<CPGame::BoardPosition>&  getCoveredFields(GameCtx& ctx) override;
    virtual std::unique_ptr<BoardSprite> clone() const override {
        return std::unique_ptr<BoardSprite>(new Wall(*this));
    }

    
};
#endif /* Wall_hpp */

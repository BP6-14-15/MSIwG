//
//  Player.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <stdlib.h>
#include "Texture.hpp"
#include "BoardSprite.hpp"

class Player: public BoardSprite {
    std::shared_ptr<Texture> idTexture;
    
public:
    int boardSize;
    int id = 0; // 0 - criminal, no id

    std::vector<CPGame::BoardMoveDirection> moveQueue;
    CPGame::PlayerType type;
    
    Player(Drawing::DrawingCtx& ctx, int id, CPGame::BoardPosition pos, CPGame::PlayerType type, int boardSize);
    ~Player();
    
    const Texture* getIdTexture() {
        return idTexture.get();
    }
    
    virtual void draw(Drawing::DrawingCtx& ctx, int squareSize) override;
    virtual const std::vector<CPGame::BoardPosition>& update(GameCtx& ctx, const PlayState& playState) override;
    virtual const std::vector<CPGame::BoardPosition>& getCoveredFields(GameCtx& ctx) override;
    virtual std::unique_ptr<BoardSprite> clone() const override {
        return std::unique_ptr<BoardSprite>(new Player(*this));
    }

    void setCoveredField(const CPGame::BoardPosition& pos);
};
#endif /* Player_hpp */

//
//  Gate.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Gate_hpp
#define Gate_hpp

#include <stdio.h>
#include <vector>
#include <SDL2/SDL.h>
#include "BoardSprite.hpp"

class Gate: public BoardSprite {
    int wallOffset;
    void moveNext(const GameCtx& ctx);
    void movePrevious(const GameCtx& ctx);
    void updatePosition(const GameCtx& ctx);
    std::vector<std::shared_ptr<SDL_Rect>> toDraw;
public:
    
    Gate(bool backwards, int wallOffset, int length, const GameCtx& ctx);
    ~Gate(); 
    
    // right - moving forward, bottom - backwards
    virtual const std::vector<CPGame::BoardPosition>& update(GameCtx& ctx) override;
    virtual const std::vector<CPGame::BoardPosition>& getCoveredFields(GameCtx& ctx) override;
    
    void changeDirection();
    
    virtual void draw(Drawing::DrawingCtx& ctx, int squareSize) override;
    virtual std::unique_ptr<BoardSprite> clone() const override;

};
#endif /* Gate_hpp */

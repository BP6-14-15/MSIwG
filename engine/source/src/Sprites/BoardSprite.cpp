//
//  BoardSprite.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 11.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "BoardSprite.hpp"

void BoardSprite::draw(Drawing::DrawingCtx &ctx, int squareSize) {
    ctx.pushStack();
    ctx.translate({(pos.x - 1) * squareSize, (pos.y - 1) * squareSize});
    
    int width = squareSize;
    int height = squareSize;
    switch (dir) {
        case BoardDirection::right:
            width = squareSize * length;
            break;
        case BoardDirection::bottom:
            height = squareSize * length;
            break;
            
    }
    ctx.drawWideRectFill({width, height}, squareSize);
    
    ctx.popStack();
    
}

//
//  Gate.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "Gate.hpp"
#include <vector>
#include <random>
#include <set>

using namespace std;
using namespace CPGame;

Gate::~Gate() {
    
}

Gate::Gate(bool backwards, int wallOffset, int length, const GameCtx& ctx):
    BoardSprite({0, 0}, backwards ? BoardDirection::bottom : BoardDirection::right, length),
    wallOffset(wallOffset) {
        
    assert(wallOffset <= ctx.gameConf.boardSize * 4);
    
    updatePosition(ctx);
}

const std::vector<CPGame::BoardPosition>&  Gate::getCoveredFields(GameCtx& ctx) {
    return coveredFields;
}

void Gate::updatePosition(const GameCtx& ctx) {
    toDraw.clear();
    coveredFields.clear();
    
    
    SDL_Rect rect = {0, 0, 1, 1};
    int limit = ctx.gameConf.boardSize + 1;
    int current = wallOffset;
    
    for (int i = 0; i < length; i++) {
        current += 1;
        int div = current / limit;
        int remainder = current % limit;
        switch (div) {
            case 1:
                rect.x = limit;
                rect.y = remainder;
                break;
            case 2:
                rect.x = limit - remainder;
                rect.y = limit;
                break;
            case 3:
                rect.x = 0;
                rect.y = limit - remainder;
                break;
            default:
                rect.x = remainder;
                rect.y = 0;
                break;
        }
        coveredFields.push_back({rect.x, rect.y});
        toDraw.push_back(make_shared<SDL_Rect>(rect));
    }
}

const std::vector<CPGame::BoardPosition>& Gate::update(GameCtx& ctx, const PlayState& playState) {
    if (ctx.rollDiceWithProbability(ctx.gameConf.pGDC)) {
        changeDirection();
    }

    if (!ctx.rollDiceWithProbability(ctx.gameConf.pGM)) {
        return coveredFields;
    }
    if (dir == BoardDirection::right) {
        moveNext(ctx);
    } else {
        movePrevious(ctx);
    }
    return coveredFields;

}

void Gate::draw(Drawing::DrawingCtx& ctx, int squareSize) {
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255 );

    ctx.pushStack();
    ctx.translate({-squareSize, -squareSize}); // offset
    for(auto& rect: toDraw) {
        ctx.drawSDLWideRectFill({rect->x * squareSize, rect->y * squareSize, rect->w * squareSize, rect->h * squareSize}, 6);
    }
    ctx.popStack();
    
}

void Gate::moveNext(const GameCtx& ctx) {
    
    wallOffset += 1;
    if (wallOffset == 4 * ctx.gameConf.boardSize + 4) {
        wallOffset = 0;
    }
   
    updatePosition(ctx);
}

void Gate::movePrevious(const GameCtx& ctx) {
    wallOffset -= 1;
    if (wallOffset == 0) {
        wallOffset = 4 * ctx.gameConf.boardSize + 4;
    }
    updatePosition(ctx);
}

std::unique_ptr<BoardSprite> Gate::clone() const {
    return std::unique_ptr<BoardSprite>(new Gate(*this));
}

void Gate::changeDirection()  {
    if (dir == BoardDirection::right) {
        dir = BoardDirection::bottom;
    } else {
        dir = BoardDirection::right;
    }
}

vector<BoardPosition> Gate::boardNeighbourFields(const GameCtx& ctx) const {
    int boardSize = ctx.gameConf.boardSize;
    set<BoardPosition> neighbours;
    for(auto& field: coveredFields) {
        if ((field.y == 0 || field.y == boardSize + 1) && (field.x == 0 || field.x == boardSize + 1)) {
            continue;
        }
        if (field.x == 0) {
            neighbours.insert({1, field.y});
        } else if (field.x == boardSize + 1) {
            neighbours.insert({boardSize, field.y});
        } else if (field.y == 0) {
            neighbours.insert({field.x, 1});
        } else {
            neighbours.insert({field.x, boardSize});
        }

    }
    vector<BoardPosition> res;
    res.assign(neighbours.begin(), neighbours.end());
    return res;
}


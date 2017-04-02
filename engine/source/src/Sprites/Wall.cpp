//
//  Wall.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 11.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "Wall.hpp"

using namespace CPGame;

Wall::Wall(BoardPosition pos, BoardDirection dir, BoardMoveDirection moveDir, int length, const GameCtx& ctx): BoardSprite(pos, dir, length), moveDir(moveDir) {
    assert(pos.x > 0 && pos.y > 0);
    // precondition - walls must not go beyond board - automatically adjust
    switch (dir) {
        case BoardDirection::right:
            if (pos.x + length > ctx.gameConf.boardSize) {
                pos.x = ctx.gameConf.boardSize - length;
                this->pos.x = pos.x;
            }
            assert(pos.y <= ctx.gameConf.boardSize); // this has to be met before initialization
            break;
        case BoardDirection::bottom:
            if (pos.y + length > ctx.gameConf.boardSize) {
                pos.y = ctx.gameConf.boardSize - length;
                this->pos.y = pos.y;
            }
            assert(pos.x <= ctx.gameConf.boardSize); // this has to be met before initialization
            break;
    }
    
    if (dir == BoardDirection::right) {
        for (int i = 0; i < length; i++) {
            coveredFields.push_back({pos.x + i, pos.y});
        }
    } else {
        for (int i = 0; i < length; i++) {
            coveredFields.push_back({pos.x, pos.y + i});
        }
    }
}


void Wall::draw(Drawing::DrawingCtx& ctx, int squareSize) {
    SDL_SetRenderDrawColor(ctx.renderer, 40, 50, 60, 255);
    BoardSprite::draw(ctx, squareSize);
}

const std::vector<BoardPosition>& Wall::getCoveredFields(GameCtx& ctx) {
    return coveredFields;
}

const std::vector<BoardPosition>& Wall::update(GameCtx& ctx, const PlayState& playState) {
    if (ctx.rollDiceWithProbability(ctx.gameConf.pWDC)) {
        switch (moveDir) {
            case BoardMoveDirection::left:
                moveDir = BoardMoveDirection::top;
                break;
            case BoardMoveDirection::top:
                moveDir = BoardMoveDirection::right;
                break;
            case BoardMoveDirection::right:
                moveDir = BoardMoveDirection::bottom;
                break;
            case BoardMoveDirection::bottom:
                moveDir = BoardMoveDirection::left;
                break;
            case BoardMoveDirection::none:
                // move dir none, walls are not doing none moves
                assert(false);
                break;
        }
    }
    
    auto coveredFields = this->coveredFields;
    auto pos = this->pos;
    auto moveDir = this->moveDir;
    if (ctx.rollDiceWithProbability(ctx.gameConf.pWM)) {
        coveredFields.clear();
        if (moveDir == BoardMoveDirection::left) {
            if (pos.x == 1) {
                moveDir = BoardMoveDirection::right;
                pos.x += 1;
            } else {
                pos.x -= 1;
            }
        } else if (moveDir == BoardMoveDirection::right) {
            if (pos.x + (dir == BoardDirection::right ? length : 1) > ctx.gameConf.boardSize) {
                moveDir = BoardMoveDirection::left;
                pos.x -= 1;
            } else {
                pos.x += 1;
            }
        } else if (moveDir == BoardMoveDirection::top) {
            if (pos.y == 1) {
                moveDir = BoardMoveDirection::bottom;
                pos.y += 1;
            } else {
                pos.y -= 1;
            }
        } else if (moveDir == BoardMoveDirection::bottom) {
            if (pos.y + (dir == BoardDirection::bottom ? length : 1) > ctx.gameConf.boardSize) {
                moveDir = BoardMoveDirection::top;
                pos.y -= 1;
            } else {
                pos.y += 1; 
            }
        } else {
            // move dir none, walls are not doing none move
            assert(false);
        }
        
        if (dir == BoardDirection::right) {
            for (int i = 0; i < length; i++) {
                coveredFields.push_back({pos.x + i, pos.y});
            }
        } else {
            for (int i = 0; i < length; i++) {
                coveredFields.push_back({pos.x, pos.y + i});
            }
        }
    }
    
    
    for(auto& field: coveredFields) {
        for (int i = ctx.cachePlayerIndex; i < ctx.stateCache.size(); i++) {
            auto& pos = ctx.stateCache[i].front();
            if (pos.x == field.x && pos.y == field.y) {
                return this->coveredFields;
            }
        }
    }
    this->coveredFields = coveredFields;
    this->pos = pos;
    this->moveDir = moveDir;
    
    
    return this->coveredFields;
}

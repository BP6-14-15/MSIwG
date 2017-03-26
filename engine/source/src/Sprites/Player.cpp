//
//  Player.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "Player.hpp"

using namespace CPGame;

Player::~Player() {
    
}

Player::Player(Drawing::DrawingCtx& ctx, int id, BoardPosition pos, PlayerType type, int boardSize):
    BoardSprite(pos, BoardDirection::right, 1), type(type), boardSize(boardSize) {
        
    assert(pos.x > 0 && pos.y > 0);
    assert(pos.x <= boardSize && pos.y <= boardSize);
    coveredFields = {pos};
    this->id = type == PlayerType::police ? id : 0;
    idTexture = std::make_shared<Texture>();
    SDL_Color clr = {0, 0, 0, 255};
    uint16_t txt[] = { static_cast<uint16_t>(id + 48), 0};
    idTexture->loadFromRenderedText(txt, clr, ctx.renderer, ctx.textFont);
        
}

void Player::draw(Drawing::DrawingCtx& ctx, int squareSize) {
    if (type == PlayerType::criminal) {
        SDL_SetRenderDrawColor(ctx.renderer, 33, 66, 99, 255);
    } else {
        SDL_SetRenderDrawColor(ctx.renderer, 255, 0, 0, 255);
    }
    
    int x = (pos.x - 1) * squareSize;
    int y = (pos.y - 1) * squareSize;
    ctx.drawSDLWideRectFill({x, y, squareSize, squareSize}, 2);
    
    SDL_SetRenderTarget(ctx.renderer, nullptr);
    SDL_Point& pt = ctx.getFromStack();
    SDL_Rect rect = {pt.x + x+ 3, pt.y + y + 3, squareSize - 6, squareSize - 6};
    SDL_RenderCopyEx(ctx.renderer, idTexture->mTexture, nullptr, &rect, 0, nullptr, SDL_FLIP_NONE);

}


const std::vector<BoardPosition>& Player::update(GameCtx& ctx) {
    
    BoardMoveDirection dir;

    if (moveQueue.empty()) {
        return coveredFields;
    } else {
        dir = moveQueue.back();
        moveQueue.pop_back();
    }
    
    auto pos = this->pos;
    switch (dir) {
        case BoardMoveDirection::left:
            pos.x -= 1;
            break;
        case BoardMoveDirection::top:
            pos.y -= 1;
            break;
        case BoardMoveDirection::right:
            pos.x += 1;
            break;
        case BoardMoveDirection::bottom:
            pos.y += 1;
            break;
        case BoardMoveDirection::none:
            return coveredFields;
    }
    
    // wall colision - no move
    for (int i = 0; i < ctx.cacheGateIndex; i++) {
        auto& covered = ctx.stateCache[i];
        for(auto& field: covered) {
            if (field == pos) {
                return coveredFields;
            }
        }
    }
    
    if (type == PlayerType::criminal) {
        if (pos.x > boardSize || pos.y > boardSize || pos.x < 1 || pos.y < 1) {
            // check for gate
            for (int i = ctx.cacheGateIndex; i < ctx.cachePlayerIndex; i++) {
                const auto& covered = ctx.stateCache[i];
                for(auto& field: covered) {
                    if (field == pos) {
                        // end game
                        ctx.colisionCtx.thiefDidEscape = true;
                    }
                }
            }
            
            // cant move to border
            if (!ctx.colisionCtx.thiefDidEscape) {
                return coveredFields;
                
            }
        }
        
    } else {
        if (pos.x > boardSize || pos.y > boardSize || pos.x < 1 || pos.y < 1) {
            // cant move to border
            return coveredFields;
        } else if (pos.x == boardSize || pos.y == boardSize) {
            // cant move next to gate
            for (int i = ctx.cacheGateIndex; i < ctx.cachePlayerIndex; i++) {
                auto& covered = ctx.stateCache[i];
                for(auto& field: covered) {
                    if ((field.x == boardSize + 1 && field.y == pos.y) || (field.y == boardSize + 1 && field.x == pos.x)) {
                        // too close to gate
                        return coveredFields;
                    }
                }
            }
        } else if (pos.x == 1 || pos.y == 1) {
            // cant move next to gate
            for (int i = ctx.cacheGateIndex; i < ctx.cachePlayerIndex; i++) {
                auto& covered = ctx.stateCache[i];
                for(auto& field: covered) {
                    if ((field.x == 0 && field.y == pos.y) || (field.y == 0 && field.x == pos.x)) {
                        // too close to gate
                        return coveredFields;
                    }
                }
            }
        }
    }
    
    this->pos = pos;
    coveredFields = {pos}; 
    return coveredFields;
}
const std::vector<BoardPosition>& Player::getCoveredFields(GameCtx& ctx) {
    return coveredFields;
}


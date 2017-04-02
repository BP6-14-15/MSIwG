//
//  PlayStateInit.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 23.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "PlayState.hpp"
#include <assert.h>
#include "Wall.hpp"
#include "UIButton.hpp"
#include "Gate.hpp"
#include "Drawing.hpp"
#include "GraphicsContext.hpp"
#include "CPGame.hpp"
#include "Utils.hpp"

using namespace CPGame;
using namespace std;

GameConfiguration DefaultGameConfiguration() {
    GameConfiguration gc;
    return gc;
}

PlayState::~PlayState() {
    
}

PlayState::PlayState(std::shared_ptr<GraphicsCtx> ctx, std::shared_ptr<GameCtx> gameCtx, std::function<void(const PlayStateEvent& e)> delegate)
: GameState(ctx), gameCtx(gameCtx), drawingCtx(*ctx->drawingCtx), delegate(delegate) {
    
    boardStateCache.boardSize = gameCtx->gameConf.boardSize;

    computeBoardPosition(gameCtx->gameConf);
    
    initBoard(gameCtx->gameConf);
    
    
    SDL_Rect rect = {16, 16, 150, 50};
    auto startTxt = u"Start";
    startButton = new UIButton(drawingCtx, (uint16_t*)startTxt, rect);
    rect.y += 58;
    auto reloadTxt = u"Reload";
    reloadButton = new UIButton(drawingCtx, (uint16_t*)reloadTxt, rect);
    reloadButton->pushCallback = bind(&PlayState::reloadBtnAction, this, placeholders::_1);
    startButton->pushCallback = bind(&PlayState::startBtnAction, this, placeholders::_1);
    
    arrowUpTexture = make_unique<Texture>();
    arrowDownTexture = make_unique<Texture>();
    arrowLeftTexture = make_unique<Texture>();
    arrowRightTexture = make_unique<Texture>();
    pointTexture = make_unique<Texture>();
    
    SDL_Color arrowsColor = {0, 0, 0, 255};
    
    
    const uint16_t arrowUpTxt[] = {u'\u2191', 0};
    const uint16_t arrowDownTxt[] = {u'\u2193', 0};
    const uint16_t arrowLeftTxt[] = {u'\u2190', 0};
    const uint16_t arrowRightTxt[] = {u'\u2192', 0};
    const uint16_t pointTxt[] = {u'\u2E30', 0};

    
    arrowUpTexture->loadFromRenderedText(arrowUpTxt, arrowsColor, drawingCtx.renderer, drawingCtx.textFontBig);
    arrowDownTexture->loadFromRenderedText(arrowDownTxt, arrowsColor, drawingCtx.renderer, drawingCtx.textFontBig);
    arrowLeftTexture->loadFromRenderedText(arrowLeftTxt, arrowsColor, drawingCtx.renderer, drawingCtx.textFont);
    arrowRightTexture->loadFromRenderedText(arrowRightTxt, arrowsColor, drawingCtx.renderer, drawingCtx.textFontBig);
    pointTexture->loadFromRenderedText(pointTxt, arrowsColor, drawingCtx.renderer, drawingCtx.textFontBig);
    
    resultTexture = make_unique<Texture>();
    
    resultTexture->drawingRect = {16, 16, 600, 100};
    
    conf = make_unique<PlayConfiguration>();
    
}

void PlayState::initBoard(const GameConfiguration& gc) {
    BoardPosition pos = {1, 1};
    if (gc.nWalls <= 0 && gc.nPolice <= 0 && gc.nGates <= 0) {
        exception(PlayStateException(PlayStateExceptionType::incorrectConfiguration));
    }
    double seed;
    
    if (gc.customSeed) {
        seed = *gc.customSeed;
    } else {
        random_device dev;
        seed = dev();
    }
        
    std::mt19937 randomGenerator(seed);
    printf("Board seed: %f\n", seed);
    
    boardSprites.clear();
    playerSprites.clear();
    gameCtx->stateCache.clear();
    
    uniform_int_distribution<> sizeRand(1, gc.boardSize);
    
    // important - add Walls first for collision
    // - so that they can first take spots which players would like to move to as well
    for (int i = 0; i < gc.nWalls; i++) {
        bool direction = rollDiceWithProbability(50, randomGenerator);
        pos.x = sizeRand(randomGenerator);
        pos.y = sizeRand(randomGenerator);
        boardSprites.push_back(make_unique<Wall>(pos, direction ? BoardDirection::bottom : BoardDirection::right, BoardMoveDirection::right, gameCtx->gameConf.lWalls, *gameCtx));
        gameCtx->stateCache.push_back(boardSprites.back()->getCoveredFields(*gameCtx));
        
    }
    
    pos = {10, 10};
    gameCtx->cacheGateIndex = (int)boardSprites.size();
    
    std::uniform_int_distribution<> gateRand(0, 4 * gc.boardSize);
    for (int i = 0; i < gc.nGates; i++) {
        boardSprites.push_back(make_unique<Gate>(false, gateRand(randomGenerator), gc.wGates, *gameCtx));
        gameCtx->stateCache.push_back(boardSprites.back()->getCoveredFields(*gameCtx));
    }
    
    gameCtx->cachePlayerIndex = (int)boardSprites.size();
    for (int i = 0; i < gc.nPolice + 1; i++) {
        bool addedFlag = false;
        
        while (!addedFlag) {
            addedFlag = true;
            pos.x = sizeRand(randomGenerator);
            pos.y = sizeRand(randomGenerator);
            
            for(auto& playerSprite: playerSprites) {
                vector<BoardPosition> fields;
                
                if (playerSprite->type == PlayerType::police && i != gc.nPolice + 1) {
                    fields = CPGame::allSurroundings(playerSprite->pos); // all neighbour fields only for criminal
                } else {
                    fields.push_back(playerSprite->pos);
                }
                
                for(auto& field: fields) {
                    if (field == pos) {
                        addedFlag = false;
                        break;
                    }
                }
            }
            
            for(int i = 0; i < gameCtx->cacheGateIndex; i++) {
                auto& fields = boardSprites[i]->getCoveredFields(*gameCtx);
                for(auto& field: fields) {
                    if (field == pos) {
                        addedFlag = false;
                        break;
                    }
                }
            }
        }
        playerSprites.push_back(make_unique<Player>(drawingCtx, i + 1, pos, PlayerType::police, gc.boardSize));
    }
    playerSprites.back()->type = PlayerType::criminal;
    playerSprites.back()->id = 0;
    gameCtx->cachePlayerIndex = (int)boardSprites.size();
    gameCtx->cacheCriminalIndex = (int)boardSprites.size() + (int)playerSprites.size() - 1; // criminal added as last
    
    for(auto& sprite: playerSprites) {
        gameCtx->stateCache.push_back(sprite->getCoveredFields(*gameCtx));
        
    }
    
}


void PlayState::computeBoardPosition(const GameConfiguration& gc) {
    int _w = (drawingCtx.screenWidth - MARGINS.left - MARGINS.right) / gc.boardSize;
    int _h = (drawingCtx.screenHeight - MARGINS.top - MARGINS.bottom ) / gc.boardSize;
    
    squareSize =  _w < _h ? _w : _h;
    if (squareSize > boardSquareMaxSize) {
        squareSize = boardSquareMaxSize;
    }
    
    int middleX = (drawingCtx.screenWidth + MARGINS.left - MARGINS.right) / 2;
    int middleY = (drawingCtx.screenHeight + MARGINS.top - MARGINS.bottom) / 2;
    
    boardStartX = (middleX - ((gc.boardSize / 2) * squareSize));
    boardStartY = (middleY - ((gc.boardSize / 2) * squareSize));
    boardWidth = gc.boardSize * squareSize;
    boardHeight = gc.boardSize * squareSize;
    
}

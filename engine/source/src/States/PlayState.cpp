//
//  PlayState.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//


#include "PlayState.hpp"
#include "Wall.hpp"
#include "UIButton.hpp"
#include "Gate.hpp"
#include "Drawing.hpp"
#include "GraphicsContext.hpp"

using namespace std;
using namespace Drawing;
using namespace CPGame;


void PlayState::nextPhase() {
    if (conf->currentPhase == PlayPhase::first) {
        boardStateCache.stateHistory.clear();

        populateBoardState();

        conf->currentPhase = PlayPhase::second;
    } else if (conf->currentPhase == PlayPhase::second) {

        const auto txt = u"Second round.";
        SDL_Color black = {0, 0, 0, 255};
        resultTexture->loadFromRenderedText((uint16_t*)txt, black, drawingCtx.renderer, drawingCtx.textFontBig);
        
        conf->currentPhase = PlayPhase::third;
        
    } else if (conf->currentPhase == PlayPhase::third) {
        boardStateCache.stateHistory.clear();
        gameCtx->updateManager.switchPlayers();

        playerSprites = vector<unique_ptr<Player>>(make_move_iterator(conf->initialPlayerSprites.begin()), make_move_iterator(conf->initialPlayerSprites.end()));
        boardSprites = vector<unique_ptr<BoardSprite>>(make_move_iterator(conf->initialBoardSprites.begin()), make_move_iterator(conf->initialBoardSprites.end()));
        conf->initialPlayerSprites.clear();
        conf->initialBoardSprites.clear();
        populateBoardState();


        conf->currentPhase = PlayPhase::fourth;
        
    } else {
        playerSprites.clear();
        boardSprites.clear();
        
        const auto txt = u"The End. Click LMB or ESC.";
        SDL_Color black = {0, 0, 0, 255};
        resultTexture->loadFromRenderedText((uint16_t*)txt, black, drawingCtx.renderer, drawingCtx.textFontBig);

        conf->currentPhase = PlayPhase::fifth;
    }
}

void PlayState::updatePlayers() {
    
    BoardPlayerUpdateRequest policeUpdateRequest;
    BoardPlayerUpdateRequest criminalUpdateRequest;
    
    
    int cacheIndex = gameCtx->cachePlayerIndex;
    for(; cacheIndex < gameCtx->cacheCriminalIndex; cacheIndex++) {
        policeUpdateRequest.objectIndexes.push_back(cacheIndex);
    }
    criminalUpdateRequest.objectIndexes.push_back(cacheIndex);

//    auto start = chrono::high_resolution_clock::now();


    auto [policemanResponse, criminalResponse] = gameCtx->updateManager.makeRequest(boardStateCache, policeUpdateRequest, criminalUpdateRequest);
//    auto end = chrono::high_resolution_clock::now();

//    std::cout << "" << chrono::duration_cast<chrono::milliseconds>(end - start).count() << std::endl;

    if (policemanResponse) {
        if (policemanResponse->moveDirection.size() != policeUpdateRequest.objectIndexes.size()) {
            cerr << "Client function error, incorrect amount of moves given for police. Ignoring all of them..." << endl;
        } else {
            int i = 0;
            for(auto& directions: policemanResponse->moveDirection) {
                if (directions.size() != policeUpdateRequest.numberOfMovesRequired) {
                } else {
                    playerSprites[policeUpdateRequest.objectIndexes[i] - boardSprites.size()]->moveQueue = directions;
                    
                }
                i++;
            }
        }
    }
    
    if (criminalResponse) {
        if (criminalResponse->moveDirection.size() != criminalUpdateRequest.objectIndexes.size()) {
            cerr << "Client function error, incorrect amount of moves given for criminal. Ignoring all of them..." << endl;
        } else {
            int i = 0;
            for(auto& directions: criminalResponse->moveDirection) {
                if (directions.size() != criminalUpdateRequest.numberOfMovesRequired) {
                } else {
                    playerSprites[criminalUpdateRequest.objectIndexes[i] - boardSprites.size()]->moveQueue = directions;
                }
                i++;
            }
        }
    }
}

void PlayState::populateBoardState() {
    BoardState boardState;    
    
    int cacheIndex = 0;
    for(; cacheIndex < gameCtx->cachePlayerIndex; cacheIndex++) {
        BoardObject bObj;
        
        bObj.coveredFields = gameCtx->stateCache[cacheIndex];
        boardState.objects.push_back(bObj);
        
    }
    
    for(auto& sprite: playerSprites) {
        BoardObject bObj;
        bObj.type = BoardObjectType::player;
        PlayerObjectData pData;
        pData.type = sprite->type;
        bObj.data.player = pData;
        bObj.coveredFields = gameCtx->stateCache[cacheIndex];
        boardState.objects.push_back(bObj);
        cacheIndex += 1;
        
    }
    
    boardStateCache.stateHistory.push_back(boardState);
    
}

void PlayState::update() {
    if (updateCounter > UPDATE_INTERVAL) {
        updateCounter = 0;
        if (gameCtx->colisionCtx.thiefWasCaught || gameCtx->colisionCtx.thiefDidEscape) {
            // collision are checked further, but we still want to render position of policeman catching thief
            //      or thief in the gate, so we still need one move update interval to render those positions
            //      will report collision with next update
            nextPhase();
            gameCtx->colisionCtx.reset();
            return;
        }
        
        updateClock += 1;
    
        
        // reaching clock interval we fetch player moves
        // similar method as with collision, first we make the last move, render it
        // and then with following update we request moves, so we don't make any other updates 
        if ((updateClock % CLOCK_INTERVAL) == 0) {
            if (boardStateCache.stateHistory.size() > CLOCK_INTERVAL - 1) {
                boardStateCache.stateHistory = vector<BoardState>(boardStateCache.stateHistory.end() - CLOCK_INTERVAL + 1,
                                                                  boardStateCache.stateHistory.end()
                                                                  );
            }
            
            updatePlayers();
            
        } else {
            int cacheIndex = 0;
            for(auto& sprite: boardSprites) {
                gameCtx->stateCache[cacheIndex] = sprite->update(*gameCtx);
                cacheIndex += 1;
                
            }
            for(auto& sprite: playerSprites) {
                gameCtx->stateCache[cacheIndex] = sprite->update(*gameCtx);
                cacheIndex += 1;
                
            }
            
            // check if criminal caught
            for(int i = 0; i < playerSprites.size() - 1; i++) {
                const auto& sprite = playerSprites[i];
                std::vector<BoardPosition> catchRange;
                catchRange.push_back({sprite->pos.x, sprite->pos.y});
                catchRange.push_back({sprite->pos.x + 1, sprite->pos.y});
                catchRange.push_back({sprite->pos.x - 1, sprite->pos.y});
                catchRange.push_back({sprite->pos.x, sprite->pos.y + 1});
                catchRange.push_back({sprite->pos.x, sprite->pos.y - 1});
                
                for(auto& otherSprite: playerSprites) {
                    if (otherSprite->type == PlayerType::criminal) {
                        for(auto& rangeField: catchRange) {
                            if (rangeField == otherSprite->pos) {
                                cout << "Thief caught!" << endl;
                                gameCtx->colisionCtx.thiefWasCaught = true;
                                return;
                            }
                        }
                    }
                }
            }
            
            populateBoardState();
        }
        
    } else {
        updateCounter += 1;
    }
   

}

void PlayState::draw() {
    
    if (conf->currentPhase == PlayPhase::first) { // choosing board
        startButton->draw(drawingCtx);
        reloadButton->draw(drawingCtx);

        
    } else if (conf->currentPhase == PlayPhase::third) { // second round text
        drawingCtx.drawTextureAbsolute(*resultTexture);
        
        updateCounter += 1;
        if (updateCounter > UPDATE_INTERVAL * 5) {
            nextPhase();
        }
        
        return;
        
    } else if (conf->currentPhase == PlayPhase::fifth) { // end screen
        drawingCtx.drawTextureAbsolute(*resultTexture);
        
        return;
    } else { // gameplay - update players etc
        update();
    }

    drawingCtx.setDrawColor(0, 255, 0, 255);
    
    drawingCtx.pushStack(); // initial position
    
    drawingCtx.translate({boardStartX, boardStartY});
    
    
    // draw board
    drawingCtx.pushStack();
    
    for (int i = 0; i < gameCtx->gameConf.boardSize; i++) {
        drawingCtx.pushStack();
        for (int j = 0; j < gameCtx->gameConf.boardSize; j++) {
            drawingCtx.drawRect({squareSize, squareSize});
            
            drawingCtx.translateX(squareSize);
        }
        drawingCtx.popStack();
        drawingCtx.translateY(squareSize);
    }
    
    drawingCtx.popStack();
    
    
    drawingCtx.setDrawColor(0, 0, 0, 255);
    // END board
    
    
    // board border offset
    drawingCtx.pushStack();
    
    drawingCtx.translate({-squareSize, -squareSize});
    int doubleSize = squareSize * 2;
    drawingCtx.drawWideRect({boardWidth + doubleSize, boardHeight + doubleSize}, 5, squareSize - 1);

    drawingCtx.setDrawColor(255, 255, 255, 255);
    
    drawingCtx.popStack();
    // END board border offset
    
    
    
    for(auto& sprite: boardSprites) {
        sprite->draw(drawingCtx, squareSize);
        
    }
    
    if (conf->currentPhase == PlayPhase::first) { // choosing board, render only players
        for(auto& sprite: playerSprites) {
            sprite->draw(drawingCtx, squareSize);
        }
        
    } else {
        SDL_Rect moves = { 10, 10, 15, 25};
        
        for(auto& sprite: playerSprites) {
            sprite->draw(drawingCtx, squareSize);
            
            
            SDL_SetRenderTarget(drawingCtx.renderer, nullptr);
            SDL_RenderCopyEx(
                             drawingCtx.renderer,
                             sprite->getIdTexture()->mTexture,
                             nullptr,
                             &moves,
                             0,
                             nullptr,
                             SDL_FLIP_NONE
                             );
            
            moves.x += 30;
            for(auto i = sprite->moveQueue.rbegin(); i != sprite->moveQueue.rend(); i++) {
                Texture* arrow = nullptr;
                switch (*i) {
                    case BoardMoveDirection::bottom:
                        arrow = arrowDownTexture.get();
                        break;
                    case BoardMoveDirection::top:
                        arrow = arrowUpTexture.get();
                        break;
                    case BoardMoveDirection::left:
                        arrow  = arrowLeftTexture.get();
                        break;
                    case BoardMoveDirection::right:
                        arrow = arrowRightTexture.get();
                        break;
                    case BoardMoveDirection::none:
                        arrow = pointTexture.get();
                        break;
                }
                SDL_SetRenderTarget(drawingCtx.renderer, nullptr);
                SDL_RenderCopyEx(
                                 drawingCtx.renderer,
                                 arrow->mTexture,
                                 nullptr,
                                 &moves,
                                 0,
                                 nullptr,
                                 SDL_FLIP_NONE
                                 );
                moves.x += 30;
            }
            
            moves.x = 10;
            moves.y += 35;
            
        }
    }
    
    
    
    drawingCtx.popStack(); // back to initial position
}

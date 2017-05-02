//
//  Context.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Context_hpp
#define Context_hpp

#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include "StateMachine.hpp"
#include "GameContext.hpp"
#include "Drawing.hpp"

class GameCtx; 
class StateMachine;

class GraphicsCtx: public std::enable_shared_from_this<GraphicsCtx> {
private:
    const int SCREEN_FPS = 60;
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS; // ms 
    
    std::shared_ptr<GameCtx> gameCtx;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    GraphicsCtx(const GraphicsCtx&) = delete;
    std::shared_ptr<StateMachine> stateMachine;
    std::shared_ptr<Drawing::DrawingCtx> drawingCtx;
    
    
    // main loop bool
    bool isRunning = true;
    
    
    GraphicsCtx(std::shared_ptr<GameCtx> gameCtx);
    ~GraphicsCtx();
    
    bool init(int screenMinWidth, int screenMinHeight);
    void enterMainLoop();
    
};
#endif /* Context_hpp */

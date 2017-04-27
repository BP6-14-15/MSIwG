//
//  GameState.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include "GraphicsContext.hpp"

class GraphicsCtx;

class GameState {
protected:
    std::shared_ptr<GraphicsCtx> ctx;

public:
    
    GameState(std::shared_ptr<GraphicsCtx> ctx): ctx(ctx) {
        
    }
    
    virtual ~GameState() {
        
    }
    
    virtual void draw() = 0;
    virtual void handleEvent(const SDL_Event& e) {
    
    }
    
};
#endif /* GameState_hpp */

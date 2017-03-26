//
//  Context.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 10.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <sys/time.h>
#include <chrono>
#include <optional>
#include "GraphicsContext.hpp"
#include "Drawing.hpp"
#include "PlayState.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#include "Timer.hpp"
#include "MainMenuState.hpp"
#include "StateMachine.hpp"

using namespace std;
using namespace std::chrono;

GraphicsCtx::GraphicsCtx(std::shared_ptr<GameCtx> gameCtx):gameCtx(gameCtx)  {
    
}

void GraphicsCtx::enterMainLoop() {

    Timer capTimer;
    
    
    SDL_Event e;
    while (isRunning) {
        capTimer.start();

        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ){
                isRunning = false;
            } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GetWindowSize(window, &drawingCtx->screenWidth, &drawingCtx->screenHeight);
                
            }
            stateMachine->handleEvent(e);
        }
        
        //Clear screen
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );
        stateMachine->drawActive();

        
        SDL_RenderPresent( renderer );
        
        // limit rendering
        int frameTicks = capTimer.getTicks();
        if( frameTicks < SCREEN_TICKS_PER_FRAME ){
            //Wait remaining time
            SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
        }
        
    }
}

GraphicsCtx::~GraphicsCtx() {
    TTF_CloseFont(drawingCtx->textFont);
    TTF_CloseFont(drawingCtx->textFontBig);
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    window = NULL;
    renderer = NULL;
    
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

bool GraphicsCtx::init(int screenMinWidth, int screenMinHeight) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
    }
    
    this->window = SDL_CreateWindow("Police and criminal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenMinWidth, screenMinHeight, SDL_WINDOW_SHOWN + SDL_WINDOW_RESIZABLE );
    SDL_SetWindowMinimumSize(window, screenMinWidth, screenMinHeight);
    
    if(window == NULL) {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    
    //Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    drawingCtx = make_shared<Drawing::DrawingCtx>(renderer,  screenMinWidth, screenMinHeight);

    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
    int imgFlags = IMG_INIT_PNG;
    if (!( IMG_Init( imgFlags ) & imgFlags ) ) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }
    
    if (TTF_Init() == -1) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }
    
        
    
    
    drawingCtx->textFont = TTF_OpenFont("Resources/lazy.ttf", 40);
    if (drawingCtx->textFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    
    // if small loaded, those will be as well ~.~
    drawingCtx->textFontBig = TTF_OpenFont("Resources/lazy.ttf", 60);
    drawingCtx->textFontHuge = TTF_OpenFont("Resources/lazy.ttf", 180);
    stateMachine = make_shared<StateMachine>("menu", make_shared<MainMenuState>(shared_from_this(), gameCtx));
    
    return true;
}

//
//  Timer.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 12.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "Timer.hpp"
#include <SDL2/SDL.h>

Timer::Timer() {
    startTicks = 0;
    started = false;
}

void Timer::start() {
    started = true;
    startTicks = SDL_GetTicks();

}

void Timer::stop() {
    started = false;
    
    startTicks = 0;

}

uint32_t Timer::getTicks() {
    //The actual timer time
    uint32_t time = 0;
    
    //If the timer is running
    if (started) {
       
        //Return the current time minus the start time
        time = SDL_GetTicks() - startTicks;
        
    }
    
    return time;
}

bool Timer::isStarted() {
    return started;
}

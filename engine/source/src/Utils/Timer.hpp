//
//  Timer.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 12.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Timer_hpp
#define Timer_hpp


#include <stdio.h>
#include <inttypes.h>

class Timer{
public:
    Timer();
    
    void start();
    void stop();
    
    uint32_t getTicks();
    
    bool isStarted();
    
private:
    uint32_t startTicks;

    bool started;
};

#endif /* Timer_hpp */

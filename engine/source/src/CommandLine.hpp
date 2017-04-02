//
//  CommandLine.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 24.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef CommandLine_hpp
#define CommandLine_hpp

#include <stdio.h>
#include "GameManager.hpp"

struct PlayersSource {
    void* firstPlayerLib = nullptr;
    void* secondPlayerLib = nullptr;
    
    CPGame::PlayerControllerCallback player1;
    CPGame::PlayerControllerCallback player2;
        
    PlayersSource(CPGame::PlayerControllerCallback player1, CPGame::PlayerControllerCallback player2);
    ~PlayersSource();
    
};

void parseArg(int argc, char* args[], GameConfiguration& conf, PlayersSource& src);
#endif /* CommandLine_hpp */

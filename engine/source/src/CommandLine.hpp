//
//  CommandLine.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 24.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef CommandLine_hpp
#define CommandLine_hpp

#include "GameManager.hpp"

struct PlayersSource {
    void* firstPlayerLib = nullptr;
    void* secondPlayerLib = nullptr;
    
    std::shared_ptr<GameRemotePlayer> player1;
    std::shared_ptr<GameRemotePlayer> player2;
        
    PlayersSource(std::shared_ptr<GameRemotePlayer> player1, std::shared_ptr<GameRemotePlayer> player2);
    ~PlayersSource();
    
};

void parseArg(int argc, char* args[], GameConfiguration& conf, PlayersSource& src);
#endif /* CommandLine_hpp */

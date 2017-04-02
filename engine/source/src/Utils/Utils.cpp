//
//  Utils.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 02.04.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <stdio.h>
#include "Utils.hpp"


std::vector<CPGame::BoardPosition> CPGame::surroundings(const CPGame::BoardPosition& pos) {
    std::vector<BoardPosition> surroundings;
    surroundings.push_back({pos.x, pos.y});
    surroundings.push_back({pos.x + 1, pos.y});
    surroundings.push_back({pos.x - 1, pos.y});
    surroundings.push_back({pos.x, pos.y + 1});
    surroundings.push_back({pos.x, pos.y - 1});
    return surroundings;
}

std::vector<CPGame::BoardPosition> CPGame::allSurroundings(const CPGame::BoardPosition& pos) {
    auto surroundings = CPGame::surroundings(pos);
    surroundings.push_back({pos.x - 1, pos.y - 1});
    surroundings.push_back({pos.x + 1, pos.y - 1});
    surroundings.push_back({pos.x - 1, pos.y + 1});
    surroundings.push_back({pos.x + 1, pos.y + 1});
    return surroundings;
    
}

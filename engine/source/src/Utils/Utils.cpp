//
//  Utils.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 02.04.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include <stdio.h>
#include <time.h>
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

std::string currentDate(time_t* tm){
    time_t now = time(tm);
    struct tm tstruct;
    char buf[40];
    tstruct = *localtime(&now);
    //format: day DD-MM-YYYY
    strftime(buf, sizeof(buf), "%A %d/%m/%Y", &tstruct);
    return buf;
}

std::string currentTime(time_t* tm){
    time_t now = time(NULL);
    struct tm tstruct;
    char buf[40];
    tstruct = *localtime(&now);
    //format: HH:mm:ss
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

std::string currentDateAndTime() {
    time_t tm = time(nullptr);
    auto date = currentDate(&tm);
    auto time = currentTime(&tm);
    return date + " " + time;
}


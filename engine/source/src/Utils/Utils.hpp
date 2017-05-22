//
//  Utils.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 01.04.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <vector>
#include <iostream>
#include "Board.hpp"

std::string currentTime(time_t* = nullptr);
std::string currentDate(time_t* = nullptr);
std::string currentDateAndTime();
    
template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    out << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << ", ";
    }
    out << "]";
    return out;
}

template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<std::vector<T>>& v) {
    out << "[\n";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << "\t" << v[i];
        if (i != last)
            out << ", ";
        out << std::endl;
    }
    out << "]";
    return out;
}

namespace CPGame {
    std::vector<BoardPosition> surroundings(const BoardPosition& pos);
    
    std::vector<BoardPosition> allSurroundings(const BoardPosition& pos);
}
#endif /* Utils_h */


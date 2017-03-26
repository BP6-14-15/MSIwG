//
//  GameCtxException.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 24.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef GameCtxException_h
#define GameCtxException_h

enum class GameCtxExceptionType {
    incorrectBoardSize, incorrectPoliceAmount, incorrectGatesAmount, incorrectWallsAmount
};

class GameCtxException: public std::exception {
public:
    GameCtxExceptionType type;
    
    GameCtxException(GameCtxExceptionType type): type(type) {
        
    }
    
    virtual const char* what() const throw () {
        switch (type) {
            case GameCtxExceptionType::incorrectBoardSize:
                return "incorrectBoardSize, minimum is 20";
            case GameCtxExceptionType::incorrectPoliceAmount:
                return "incorrectPoliceAmount, must be at least 1";
            case GameCtxExceptionType::incorrectGatesAmount:
                return "incorrectGatesAmount, must be at least 1";
            case GameCtxExceptionType::incorrectWallsAmount:
                return "incorrectWallsAmount, must be at least 1";
        }
    }
};

#endif /* GameCtxException_h */

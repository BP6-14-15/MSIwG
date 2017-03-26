//
//  PlayStateException.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 24.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef PlayStateException_hpp
#define PlayStateException_hpp

#include <stdio.h>
#include <exception>
enum class PlayStateExceptionType {
    incorrectConfiguration
};

class PlayStateException: public std::exception {
public:
    PlayStateExceptionType type;
    
    PlayStateException(PlayStateExceptionType type): type(type) {
        
    }
    
    virtual const char* what() const throw (){
        switch (type) {
            case PlayStateExceptionType::incorrectConfiguration:
                return "incorrectConfiguration";
        }
    }
};

#endif /* PlayStateException_hpp */

//
//  StateMachine.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef StateMachine_hpp
#define StateMachine_hpp

#include <stdio.h>
#include <map>
#include <assert.h>
#include "GraphicsContext.hpp"

class GameState;

enum class StateMachineExceptionType {
    stateNotRegistered, cantUnregisterActiveState 
};

class StateMachineException: public std::exception {
public:
    StateMachineExceptionType type;
    
    StateMachineException(StateMachineExceptionType type): type(type) {
        
    }
    
    virtual const char* what() const throw ();
};

class StateMachine {
    std::map<std::string, std::shared_ptr<GameState>> activeStates;
    GameState* active;
    std::string activeName; 
    
public:
    
    StateMachine(const std::string& initialName, std::shared_ptr<GameState> initial);
    
    void registerState(const std::string& name, std::shared_ptr<GameState> state);
    void unregister(const std::string& name); 
    
    void setActive(const std::string& name);
    
    GameState* getActive();
    
    void drawActive();
    
    void handleEvent(const SDL_Event& e);
    
};
#endif /* StateMachine_hpp */

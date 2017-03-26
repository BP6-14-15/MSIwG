//
//  StateMachine.cpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#include "StateMachine.hpp"
#include "GameState.hpp"

using namespace std;

const char* StateMachineException::what() const throw () {
    switch (type) {
        case StateMachineExceptionType::stateNotRegistered:
            return "stateNotRegistered";
        case StateMachineExceptionType::cantUnregisterActiveState:
            return "cantUnregisterActiveState";
    }
}

StateMachine::StateMachine(const std::string& initialName, std::shared_ptr<GameState> initial) {
    registerState(initialName, initial);
    active = activeStates[initialName].get();
    activeName = initialName;
}

void StateMachine::setActive(const std::string& name) {
    auto found = activeStates.find(name);
    if (found == activeStates.end()) {
        throw StateMachineException(StateMachineExceptionType::stateNotRegistered);
    }
    activeName = name;
    active = found->second.get();
    
}

GameState* StateMachine::getActive() {
    return active;
}

void StateMachine::drawActive() {
    assert(active != nullptr); // did not active any state
    active->draw();
}

void StateMachine::handleEvent(const SDL_Event& e) {
    assert(active != nullptr); // did not active any state
    active->handleEvent(e);
}

void StateMachine::registerState(const std::string& name, std::shared_ptr<GameState> state) {
    activeStates.insert(make_pair(name, state));
}

void StateMachine::unregister(const std::string &name) {
    if (name == activeName) {
        throw StateMachineException(StateMachineExceptionType::stateNotRegistered);
    }
    activeStates.erase(name);
    
}

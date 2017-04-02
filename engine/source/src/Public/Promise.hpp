//
//  Promise.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 24.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef Promise_hpp
#define Promise_hpp
#include <memory>
#include <iostream>

namespace CPGame {
template<typename T>
class Promise {
    std::shared_ptr<T> underlying = nullptr;
    std::mutex dataMutex;
public:
    
    Promise(const Promise&) = delete;
    
    Promise() {
        
    }
    const std::shared_ptr<T> getValue() {
        std::lock_guard<std::mutex> lock(dataMutex);
        return underlying;
        
    }
    
    bool setValue(const T& newData) {
        std::unique_lock<std::mutex> lock(dataMutex, std::try_to_lock);
        
        if (lock.owns_lock()) {
            underlying = std::make_shared<T>(newData);
            return true;
        } else {
            return false;
        }
    }
};
}
#endif /* Promise_hpp */

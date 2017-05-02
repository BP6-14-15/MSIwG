//
//  UIObject.hpp
//  SDLTest
//
//  Created by Damian Malarczyk on 13.03.2017.
//  Copyright © 2017 Damian Malarczyk. All rights reserved.
//

#ifndef UIObject_hpp
#define UIObject_hpp

#include <stdio.h>
#include "Drawing.hpp"

class UIObject {
public:

    virtual ~UIObject() {
        
    }
    
    virtual void draw(Drawing::DrawingCtx& ctx) = 0;
};

#endif /* UIObject_hpp */

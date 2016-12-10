//
//  utils.cpp
//  trees
//
//  Created by Lutz on 10/12/16.
//
//

#include "utils.hpp"
#include <math.h>

double utils::constrainAngle(double x){
    x = fmod(x,M_PI*2);
    while (x < 0)
        x += M_PI*2;
    return x;
}

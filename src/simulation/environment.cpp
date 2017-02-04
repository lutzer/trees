//
//  environment.cpp
//  trees
//
//  Created by Lutz on 04/02/17.
//
//

#include <complex>

#include "environment.hpp"

env::Environment::Environment(pts::Point sun, pts::BoundingBox boundingBox) {
    this->sun = sun;
    this->boundingBox = boundingBox;
};

pts::SizeInt env::calculateBinMatrixSize(pts::BoundingBox boundingBox) {
    if (fmod(boundingBox.size.width, env::BIN_SIZE) > 0 || fmod(boundingBox.size.height, env::BIN_SIZE) > 0) {
        std::string message = "bounding box sizes must be a multiple of " + std::to_string(env::BIN_SIZE);
        throw std::invalid_argument(message);
    }

    int columns = boundingBox.size.width / env::BIN_SIZE;
    int rows = boundingBox.size.height / env::BIN_SIZE;

    return {columns, rows};
}

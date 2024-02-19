#ifndef __NBD_RAST_H__
#define __NBD_RAST_H__

#include <vector>
#include <cstddef>
#include "raylib.h"

namespace NBody {

class Universe;

std::vector<Color> rasterize(size_t width, size_t height, const Universe& universe);

}

#endif
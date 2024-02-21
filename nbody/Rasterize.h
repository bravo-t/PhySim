#ifndef __NBD_RAST_H__
#define __NBD_RAST_H__

#include <vector>
#include <cstddef>
#include "raylib.h"

namespace NBody {

class Universe;

void rasterize(size_t width, size_t height, const Universe& universe, Color* pixels);


void
drawFrame(size_t width, size_t height, 
          const Universe& universe, 
          RenderTexture2D renderData, 
          Camera2D camera,
          size_t circleSize = 2);

}

#endif
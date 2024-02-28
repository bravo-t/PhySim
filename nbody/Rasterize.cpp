#include <cstring>
#include <unordered_map>
#include "Rasterize.h"
#include "Universe.h"
#include "ColorGradient.h"

namespace NBody {

struct DrawPos {
  size_t x;
  size_t y;
};

struct HashDrawPos {
  size_t operator()(const DrawPos& key) const
  {
    return std::_Hash_impl::hash(&key, sizeof(key));
  }
};

struct DrawPosEqual {
  bool operator()(const DrawPos& a, const DrawPos& b) const 
  {
    return a.x == b.x && a.y == b.y;
  }
};

void
drawFrame(size_t width, size_t height, const Universe& universe, 
          RenderTexture2D renderData, Camera2D camera, size_t circleSize)
{
  std::unordered_map<DrawPos, size_t, HashDrawPos, DrawPosEqual> drawDataMap;
  const std::deque<Planet>& planets = universe.planetData();
  double radius = universe.radius();
  size_t maxCount = 0;
  for (const Planet& p : planets) {
    double coorX = p.coordinate()[0];
    size_t posX = int((coorX / radius + 1) * width / 2);
    double coorY = p.coordinate()[1];
    size_t posY = int((coorY / radius + 1) * height / 2);
    DrawPos pos{posX, posY};
    auto found = drawDataMap.find(pos);
    if (found != drawDataMap.end()) {
      ++(found->second);
      maxCount = found->second > maxCount ? found->second : maxCount;
    } else {
      drawDataMap.insert({pos, 1});
      if (maxCount == 0) {
        maxCount = 1;
      }
    }
  }

  ColorGradient colorPicker;
  colorPicker.createDefaultHeatMapGradient();

  BeginTextureMode(renderData);
    ClearBackground(BLACK);

    BeginMode2D(camera);
      for (const auto& kv : drawDataMap) {
        Color c = colorPicker.getColorAtValue(1.0 * kv.second / maxCount);
        //printf("Adding planet on %lux%lu\n");
        DrawCircleGradient(kv.first.x, kv.first.y, circleSize, c, Fade(c, 0.1));
      }
    EndMode2D();
  EndTextureMode();

}

}
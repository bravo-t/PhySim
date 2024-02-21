#include <cstring>
#include <unordered_map>
#include "Rasterize.h"
#include "Universe.h"
#include "ColorGradient.h"

namespace NBody {

void
rasterize(size_t width, size_t height, const Universe& universe, Color* pixels)
{
  size_t maxCount = 0;
  std::vector<size_t> counter(width*height, 0);
  counter.reserve(width*height);
  const std::deque<Planet>& planets = universe.planetData();
  double radius = universe.radius();
  for (const Planet& p : planets) {
    double coorX = p.coordinate()[0];
    size_t posX = int((coorX / radius + 1) * width / 2);
    double coorY = p.coordinate()[1];
    size_t posY = int((coorY / radius + 1) * height / 2);
    size_t index = posY * width + posX;
    ++counter[index];
    maxCount = counter[index] > maxCount ? counter[index] : maxCount;
    //printf("Pos (%lu, %lu) counter %lu, max %lu\n", posX, posY, counter[index], maxCount);
  }

  //memset(pixels, 0, counter.size()*sizeof(Color));

  ColorGradient colorPicker;
  colorPicker.createDefaultHeatMapGradient();
  for (size_t i=0; i<counter.size(); ++i) {
    if (counter[i] > 0) {
      Color pixel = colorPicker.getColorAtValue(1.0 * counter[i] / maxCount);
      pixels[i] = pixel;
    } 
    double r = 1.0 * rand() / RAND_MAX;
    if (r > 0.98) {
      Color pixel = colorPicker.getColorAtValue(0.05);
      pixels[i].r = pixels[i].r + pixel.r > 255 ? 255 : pixels[i].r + pixel.r;
      pixels[i].g = pixels[i].g + pixel.g > 255 ? 255 : pixels[i].g + pixel.g;
      pixels[i].b = pixels[i].b + pixel.b > 255 ? 255 : pixels[i].b + pixel.b;
    }
  }
}

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
drawFrame(size_t width, size_t height, const Universe& universe, RenderTexture2D renderData, Camera2D camera, size_t circleSize)
{
  std::unordered_map<DrawPos, size_t, HashDrawPos, DrawPosEqual> drawDataMap;
  const std::deque<Planet>& planets = universe.planetData();
  double radius = universe.radius();
  for (const Planet& p : planets) {
    double coorX = p.coordinate()[0];
    size_t posX = int((coorX / radius + 1) * width / 2);
    double coorY = p.coordinate()[1];
    size_t posY = int((coorY / radius + 1) * height / 2);
    DrawPos pos{posX, posY};
    auto found = drawDataMap.find(pos);
    if (found != drawDataMap.end()) {
      ++(found->second);
    } else {
      drawDataMap.insert({pos, 1});
    }
  }

  ColorGradient colorPicker;
  colorPicker.createDefaultHeatMapGradient();

  BeginTextureMode(renderData);
    ClearBackground(BLACK);

    BeginMode2D(camera);
      for (const auto& kv : drawDataMap) {
        Color c = colorPicker.getColorAtValue(1.0 * kv.second / planets.size());
        //printf("Adding planet on %lux%lu\n");
        DrawCircleGradient(kv.first.x, kv.first.y, circleSize, c, Fade(c, 0.1));
      }
    EndMode2D();
  EndTextureMode();

}

}
#include "Rasterize.h"
#include "Universe.h"
#include "gui/ColorGradient.h"

namespace NBody {

std::vector<Color>
rasterize(size_t width, size_t height, const Universe& universe)
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
  }

  std::vector<Color> pixels;
  pixels.reserve(width*height);

  ColorGradient colorPicker;
  colorPicker.createDefaultHeatMapGradient();
  for (size_t c : counter) {
    pixels.push_back(colorPicker.getColorAtValue(1.0 * c / maxCount));
  }
  return pixels;
}

}
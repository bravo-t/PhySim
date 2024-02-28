#ifndef __NBD_APPOPT_H__
#define __NBD_APPOPT_H__

#include "Simulator.h"

namespace NBody {

struct AppOptions {
  /// Main options
  const char* _worldFile = nullptr;
  size_t _screenWidth = 800;
  size_t _screenHeight = 600;
  size_t _fps = 60;
  size_t _warpTick = 1000;
  /// Simulator options
  SimulationMethod _simMethod = SimulationMethod::Direct;
  double _simTick = 1;
  double _simBHTheta = 0.5;
  /// GUI options
  size_t _circleSize = 2;
};

AppOptions
processOptions(int argc, char** argv);

}

#endif
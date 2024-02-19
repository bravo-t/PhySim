#ifndef _NBD_SIMULATOR_H_
#define _NBD_SIMULATOR_H_

#include <deque>

namespace NBody {

class Planet;

enum SimulationMethod {
  Direct,

};

class Simulator {
  public:
    typedef std::deque<Planet> PlanetData;
    Simulator(SimulationMethod method = SimulationMethod::Direct);
    PlanetData run(const PlanetData& initCond);

  private:
    PlanetData directMethod(const PlanetData& initCond);

  private:
    SimulationMethod _simMethod = SimulationMethod::Direct;
    size_t           _simTickPerSecond = 1000;
};

}

#endif
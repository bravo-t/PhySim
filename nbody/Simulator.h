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
    PlanetData run(const PlanetData& startCond);

  private:
    PlanetData directMethod(const PlanetData& startCond);

  private:
    size_t _simTickPerSecond = 1000;
};

}

#endif
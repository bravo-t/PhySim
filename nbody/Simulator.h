#ifndef _NBD_SIMULATOR_H_
#define _NBD_SIMULATOR_H_

#include <deque>

namespace NBody {

class Planet;
class Universe;

enum SimulationMethod {
  Direct,
  BarnesHutAlgorithm,
};

class Simulator {
  public:
    typedef std::deque<Planet> PlanetData;
    Simulator(const Universe* universe, 
              SimulationMethod method = SimulationMethod::Direct) 
    : _simMethod(method), _universe(universe) {}
    PlanetData run(const PlanetData& initCond) const;
    void setSimTick(double tick) { _simTickPerSecond = tick; }
    void setBHTheta(double theta) { _BHTheta = theta; }

  private:
    PlanetData directMethod(const PlanetData& initCond) const;
    PlanetData BarnesHutAlgorithm(const PlanetData& initCond) const;

  private:
    SimulationMethod _simMethod = SimulationMethod::Direct;
    double           _simTickPerSecond = 1;
    double           _BHTheta = 0.5;
    const Universe*  _universe = nullptr;
};

}

#endif
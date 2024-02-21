#ifndef _NBD_UNIVERSE_H_
#define _NBD_UNIVERSE_H_

#include <deque>
#include <string>
#include "Planet.h"

namespace NBody {

class Simulator;

class Universe {
  public:
    Universe(const char* worldFile);
    std::string planetName(size_t planetId) const { return _planetNames[planetId]; }
    void simulate(const Simulator& sim);
    std::deque<Planet> planetData() const { return _planetData; }
    double radius() const { return _radius; }

  private:
    bool buildFromFile(const char* worldFile);
    void addPlanet(const std::string& fileLine);

  private:
    double _radius = .0;
    size_t _velocityScale = 1000;
    std::deque<std::string> _planetNames;
    std::deque<Planet>      _planetData;
};


}

#endif
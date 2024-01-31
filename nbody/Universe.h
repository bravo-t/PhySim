#ifndef _NBD_UNIVERSE_H_
#define _NBD_UNIVERSE_H_

#include <deque>
#include <string>
#include "Planet.h"

namespace NBody {

class Universe {
  public:
    Universe(const char* worldFile);
    void simulate();
  private:
    void buildFromFile(const char* worldFile);
    void addPlanet(const std::string& fileLine);
  private:
    double _radius = .0;
    std::deque<Planet> _planets;
};


}

#endif
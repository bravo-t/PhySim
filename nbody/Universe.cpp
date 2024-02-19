#include <fstream>
#include <cassert>
#include <vector>
#include <cstring>
#include "Universe.h"
#include "Simulator.h"

namespace NBody {

Universe::Universe(const char* worldFile)
{
  buildFromFile(worldFile);
}

bool
Universe::buildFromFile(const char* worldFile)
{
  std::ifstream infile(worldFile);
  if (!infile) {
    printf("ERROR: Cannot open %s\n", worldFile);
    return false;
  }
  std::string line;
  size_t numPlanets = 0;
  double r = .0;
  if (std::getline(infile, line)) {
    numPlanets = std::stoul(line);
  }
  if (std::getline(infile, line)) {
    r = std::stod(line);
  }
  while (std::getline(infile, line)) {
    addPlanet(line);
  }
  if (numPlanets != _planetData.size()) {
    printf("ERROR: Build imcomplete, %lu planets added, expected %lu\n", _planetData.size(), numPlanets);
    exit(1);
  }
  _radius = r;
  printf("Universe built with input file %s, %lu planets created\n", worldFile, numPlanets);
  return true;
}

inline void 
splitWithAny(const std::string& src, const char *delim,
             std::vector<std::string> &strs)
{
  char *data = strdup(src.data());
  strs.clear();
  char *saveptr(nullptr);
  if (char* res1 = strtok_r(data, delim, &saveptr)) {
    strs.push_back(res1);
    while(char *res2 = strtok_r(nullptr, delim, &saveptr)) {
      strs.push_back(std::string(res2));
    }
  }
  ::free(data);
}

void
Universe::addPlanet(const std::string& line)
{
  std::vector<std::string> strs;
  splitWithAny(line, " ", strs);
  if (strs.size() != 6) {
    return;
  }
  size_t planetId = _planetData.size();
  _planetNames.push_back(strs[5]);
  Planet p(planetId);
  p.setCoordinate(std::stod(strs[0]), std::stod(strs[1]));
  p.setVelocity(std::stod(strs[2]), std::stod(strs[3]));
  p.setMass(std::stod(strs[4]));
  _planetData.push_back(p);
}

void
Universe::simulate(const Simulator& sim)
{
  std::deque<Planet> nextState = sim.run(_planetData);
  nextState.swap(_planetData);
}


}
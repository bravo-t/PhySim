#include <cmath>
#include <cassert>
#include "Simulator.h"
#include "Planet.h"

namespace NBody {

typedef Simulator::PlanetData PlanetData;

const double G = 6.6743e-11;

static inline void
updateAcceleration(const Planet& planet, const Planet& other, 
                   double& accX, double& accY)
{
  double coorXDiff = planet.coordinate()[0] - other.coordinate()[0];
  double coorYDiff = planet.coordinate()[1] - other.coordinate()[1];
  double rSquared = coorXDiff * coorXDiff + coorYDiff * coorYDiff;
  double rSqrt = sqrt(rSquared);
  double otherAcc = G * other.mass() / rSquared;
  double scaledAcc = otherAcc / rSqrt;
  accX += coorXDiff * scaledAcc;
  accY += coorYDiff * scaledAcc;
}

void 
acceleration(size_t planetId, const PlanetData& stepCond, 
             double& accX, double& accY)
{
  accX = accY = 0;
  const Planet& planet = stepCond[planetId];
  for (size_t i=0; i<stepCond.size(); ++i) {
    if (i == planetId) {
      continue;
    }
    const Planet& other = stepCond[i];
    updateAcceleration(planet, other, accX, accY);
  }
}

PlanetData
explicitEulerTick(const PlanetData& stepCond, double simTick,
                  PlanetData& nextStep)
{
  nextStep.assign(stepCond.begin(), stepCond.end());
  for (size_t i=0; i<stepCond.size(); ++i) {
    double accX = 0;
    double accY = 0;
    acceleration(i, stepCond, accX, accY);
    const Planet& planet = stepCond[i];
    double velX = planet.velocity()[0];
    double velY = planet.velocity()[1];
    velX += accX * simTick;
    velY += accY * simTick;
    double coorX = planet.coordinate()[0];
    double coorY = planet.coordinate()[1];
    coorX += velX * simTick;
    coorY += velY * simTick;

    nextStep[i].setCoordinate(coorX, coorY);
    nextStep[i].setVelocity(velX, velY);
  }
}

PlanetData
Simulator::directMethod(const PlanetData& startCond)
{
  double simTick = 1 / _simTickPerSecond;
  PlanetData nextStep(startCond.begin(), startCond.end());
  PlanetData currentStep;
  for (size_t tickCount=0; tickCount<_simTickPerSecond; ++tickCount) {
    currentStep.swap(nextStep);
    explicitEulerTick(currentStep, simTick, nextStep);
  }
  return nextStep;
}

PlanetData
Simulator::run(const PlanetData& initCond) 
{
  switch (_simMethod) {
    case SimulationMethod::Direct:
      return directMethod(initCond);
  
    default:
      assert(false && "Incorrect simulation method");
  }
}

}
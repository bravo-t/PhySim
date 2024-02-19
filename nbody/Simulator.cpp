#include <cmath>
#include "Simulator.h"
#include "Planet.h"

namespace NBody {

typedef Simulator::PlanetData PlanetData;

const double G = 6.6743e-11;

void acceleration(size_t planetId, const PlanetData& stepCond, 
                  double& xAcc, double& yAcc)
{
  xAcc = yAcc = 0;
  const Planet& planet = stepCond[planetId];
  for (size_t i=0; i<stepCond.size(); ++i) {
    if (i == planetId) {
      continue;
    }
    const Planet& other = stepCond[i];
    double coorXDiff = planet.coordinate()[0] - other.coordinate()[0];
    double coorYDiff = planet.coordinate()[1] - other.coordinate()[1];
    double rSquared = coorXDiff * coorXDiff + coorYDiff * coorYDiff;
    double rSqrt = sqrt(rSquared);
    double otherAcc = G * other.mass() / rSquared;
    double scaledAcc = otherAcc / rSqrt;
    xAcc += coorXDiff * scaledAcc;
    yAcc += coorYDiff * scaledAcc;
  }
}



PlanetData
Simulator::directMethod(const PlanetData& startCond)
{

}

}
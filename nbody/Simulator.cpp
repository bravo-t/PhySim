#include <cmath>
#include <cassert>
#include "Simulator.h"
#include "Universe.h"
#include "BarnesHutAlgo.h"
#include "Planet.h"

namespace NBody {

typedef Simulator::PlanetData PlanetData;

const double G = 6.6743e-11;

static inline void
updateAcceleration(const Planet& planet, const Planet& other, 
                   double& accX, double& accY)
{
  double coorXDiff = other.coordinate()[0] - planet.coordinate()[0];
  double coorYDiff = other.coordinate()[1] - planet.coordinate()[1];
  double rSquared = coorXDiff * coorXDiff + coorYDiff * coorYDiff;
  double rSqrt = sqrt(rSquared);
  double totalAcc = other.mass() / rSquared;
  double scaledAcc = totalAcc / rSqrt;
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
  accX *= G;
  accY *= G;
}

void
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

    /*
    printf("DEBUG: %lu: cur vel: %.gx%.g, pos: %gx%g, acc: %.gx%.g, new vel: %.gx%.g, new pos: %.gx%.g\n", 
      i, planet.velocity()[0], planet.velocity()[1], 
      planet.coordinate()[0], planet.coordinate()[1], 
      accX, accY, nextStep[i].velocity()[0], nextStep[i].velocity()[1], 
      nextStep[i].coordinate()[0], nextStep[i].coordinate()[1]);
    */
  }
}

PlanetData
Simulator::directMethod(const PlanetData& startCond) const
{
  double simTick = 1.0 / _simTickPerSecond;
  PlanetData nextStep(startCond.begin(), startCond.end());
  PlanetData currentStep;
  double accuSimSec = 0;
  while (accuSimSec < 1.0) {
    currentStep.swap(nextStep);
    explicitEulerTick(currentStep, simTick, nextStep);
    accuSimSec += simTick;
  }
  return nextStep;
}

void
calcPlanetAccBHTree(const Planet& planet, double simTick, double theta,
                    const Universe* universe, const BarnesHutTree& BHTree,
                    double& accX, double& accY)
{
  accX = 0;
  accY = 0;
  const TreeNode& root = BHTree.node(0);
  std::vector<const TreeNode*> nodeStack(1, &root);
  while (nodeStack.empty() == false) {
    const TreeNode* node = nodeStack.back();
    nodeStack.pop_back();
    if (node->occupied() && node->_planetId == planet.id()) {
      continue;
    }
    if (node->empty() || node->_totalMass == 0) {
      continue;
    }
    const std::vector<const TreeNode*>& childNodes = BHTree.childNodes(node);
    if (node->contains(planet)) {
      nodeStack.insert(nodeStack.end(), childNodes.begin(), childNodes.end());
      continue;
    }
    double coorXDiff = node->_centerOfMass[0] - planet.coordinate()[0];
    double coorYDiff = node->_centerOfMass[1] - planet.coordinate()[1];
    double rSquared = coorXDiff * coorXDiff + coorYDiff * coorYDiff;
    double distance = sqrt(rSquared);
    //printf("DEBUG: coorXDiff=%.3f, coorYDiff=%.3f, distance=%.3f, s/d=%.3f/%.3f=%.3f, theta=%.3f, CoM=%.3fx%.3f, node mass=%.3f\n", 
    //        coorXDiff, coorYDiff, distance, node->width(), distance, node->width()/distance, theta, node->_centerOfMass[0], node->_centerOfMass[1], node->_totalMass);
    if (node->width() / distance >= theta) {
      if (childNodes.size() == 0) {
        double totalAcc = node->_totalMass / rSquared;
        double scaledAcc = totalAcc / distance;
        accX += coorXDiff * scaledAcc;
        accY += coorYDiff * scaledAcc;
        //printf("DEBUG: node %lu mass=%.3f, totalAcc=%.3f, accX=%.3f, accY=%.3f\n", 
        //  node->_nodeId, node->_totalMass, totalAcc, coorXDiff*scaledAcc, coorYDiff*scaledAcc);
      } else {
        nodeStack.insert(nodeStack.end(), childNodes.begin(), childNodes.end());
      }
    } else {
      double totalAcc = node->_totalMass / rSquared;
      double scaledAcc = totalAcc / distance;
      accX += coorXDiff * scaledAcc;
      accY += coorYDiff * scaledAcc;
      //printf("DEBUG: node %lu mass=%.3f, totalAcc=%.3f, accX=%.3f, accY=%.3f\n", 
      //  node->_nodeId, node->_totalMass, totalAcc, coorXDiff*scaledAcc, coorYDiff*scaledAcc);
    }
  }
}

void
BarnesHutTick(const PlanetData& stepCond, double simTick,
              const Universe* universe, double theta,
              PlanetData& nextStep)
{
  BarnesHutTree BHTree(stepCond, universe);
  nextStep.assign(stepCond.begin(), stepCond.end());
  for (size_t i=0; i<stepCond.size(); ++i) {
    double accX = 0;
    double accY = 0;
    const Planet& planet = stepCond[i];
    calcPlanetAccBHTree(planet, simTick, theta, universe, BHTree, accX, accY);
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
    
    //printf("DEBUG: %lu: cur vel: %.gx%.g, pos: %gx%g, acc: %.gx%.g, new vel: %.gx%.g, new pos: %.gx%.g\n", 
    //  i, planet.velocity()[0], planet.velocity()[1], 
    //  planet.coordinate()[0], planet.coordinate()[1], 
    //  accX, accY, nextStep[i].velocity()[0], nextStep[i].velocity()[1], 
    //  nextStep[i].coordinate()[0], nextStep[i].coordinate()[1]);
    
  }
}

PlanetData
Simulator::BarnesHutAlgorithm(const PlanetData& startCond) const
{
  double simTick = 1.0 / _simTickPerSecond;
  PlanetData nextStep(startCond.begin(), startCond.end());
  PlanetData currentStep;
  double accuSimSec = 0;
  while (accuSimSec < 1.0) {
    currentStep.swap(nextStep);
    BarnesHutTick(currentStep, simTick, _universe, _BHTheta, nextStep);
    accuSimSec += simTick;
  }
  return nextStep;
}

PlanetData
Simulator::run(const PlanetData& initCond) const
{
  switch (_simMethod) {
    case SimulationMethod::Direct:
      return directMethod(initCond);
    case SimulationMethod::BarnesHutAlgorithm:
      return BarnesHutAlgorithm(initCond);
    default:
      assert(false && "Incorrect simulation method");
  }
}

}
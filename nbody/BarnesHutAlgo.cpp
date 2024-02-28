#include <cassert>
#include "BarnesHutAlgo.h"
#include "Planet.h"
#include "Universe.h"

namespace NBody {

bool
TreeNode::contains(const Planet& planet) const
{
  double coorX = planet.coordinate()[0];
  double coorY = planet.coordinate()[1];
  return coorX > _ll[0] && coorX <= _ur[0] &&
         coorY > _ll[1] && coorY <= _ur[1];
}

size_t
TreeNode::childNode(const Planet& planet) const
{
  if (contains(planet) == false) {
    return static_cast<size_t>(-1);
  }
  double centerX = (_ll[0] + _ur[0]) / 2;
  double centerY = (_ll[1] + _ur[1]) / 2;
  double coorX = planet.coordinate()[0];
  double coorY = planet.coordinate()[1];
  if (coorX <= centerX && coorY > centerY) {
    return _ulNode;
  } else if (coorX <= centerX && coorY <= centerY) {
    return _llNode;
  } else if (coorX > centerX && coorY <= centerY) {
    return _lrNode;
  } else {
    return _urNode;
  }
}

void
TreeNode::updateData(const Planet& planet) 
{
  double weightedMassX = _centerOfMass[0] * _totalMass;
  double weightedMassY = _centerOfMass[1] * _totalMass;
  weightedMassX += planet.mass() * planet.coordinate()[0]; 
  weightedMassY += planet.mass() * planet.coordinate()[1]; 
  _totalMass += planet.mass();
  _centerOfMass[0] = weightedMassX / _totalMass;
  _centerOfMass[1] = weightedMassY / _totalMass;
}

size_t
TreeNode::divide(BarnesHutTree* tree)
{
  double centerX = (_ll[0] + _ur[0]) / 2;
  double centerY = (_ll[1] + _ur[1]) / 2;
  size_t ulId = tree->_treeNodes.size();
  TreeNode ulNode(ulId, centerX, _ur[1], _ll[0], centerY);
  tree->_treeNodes.push_back(ulNode);
  _ulNode = ulId;
  size_t urId = tree->_treeNodes.size();
  TreeNode urNode(urId, _ur[0], _ur[1], centerX, centerY);
  tree->_treeNodes.push_back(urNode);
  _urNode = urId;
  size_t llId = tree->_treeNodes.size();
  TreeNode llNode(llId, centerX, centerY, _ll[0], _ll[1]);
  tree->_treeNodes.push_back(llNode);
  _llNode = llId;
  size_t lrId = tree->_treeNodes.size();
  TreeNode lrNode(lrId, _ur[0], centerY, centerX, _ll[1]);
  tree->_treeNodes.push_back(lrNode);
  _lrNode = lrId;

  size_t pId = _planetId;
  _planetId = static_cast<size_t>(-1);
  return pId;
}

void
TreeNode::putPlanet(const Planet& planet)
{
  assert(occupied() == false);
  _planetId = planet.id();
}

bool
TreeNode::empty() const
{
  return _planetId == static_cast<size_t>(-1) && 
         _llNode  == static_cast<size_t>(-1) &&
         _lrNode  == static_cast<size_t>(-1) &&
         _ulNode  == static_cast<size_t>(-1) &&
         _urNode  == static_cast<size_t>(-1);
}

BarnesHutTree::BarnesHutTree(const std::deque<Planet>& planets,
                           const Universe* universe)
{
  double radius = universe->radius();
  TreeNode treeRoot(0, radius, radius, -radius, -radius);
  _treeNodes.push_back(treeRoot);
  _universe = universe;
  for (const Planet& planet : planets) {
    addPlanet(planet);
  }
}

std::vector<const TreeNode*>
BarnesHutTree::childNodes(const TreeNode* node) const
{
  std::vector<const TreeNode*> nodes;
  if (node->_llNode != static_cast<size_t>(-1)) {
    nodes.push_back(&_treeNodes[node->_llNode]);
  }
  if (node->_lrNode != static_cast<size_t>(-1)) {
    nodes.push_back(&_treeNodes[node->_lrNode]);
  }
  if (node->_ulNode != static_cast<size_t>(-1)) {
    nodes.push_back(&_treeNodes[node->_ulNode]);
  }
  if (node->_urNode != static_cast<size_t>(-1)) {
    nodes.push_back(&_treeNodes[node->_urNode]);
  }
  return nodes;
}

std::vector<size_t>
findTargetNodes(const Planet& planet, BarnesHutTree* tree)
{
  std::vector<size_t> nodes;
  size_t currentId = 0;
  while (currentId != static_cast<size_t>(-1)) {
    TreeNode& node = tree->node(currentId);
    if (node.contains(planet)) {
      nodes.push_back(currentId);
      currentId = node.childNode(planet);
    } else {
      break;
    }
  }
  return nodes;
}

static inline size_t
divideAndAdd(const Planet& planet, 
             size_t startNodeId, BarnesHutTree* tree)
{
  size_t nodeId = startNodeId;
  TreeNode& node = tree->node(nodeId);
  //printf("Dividing node %lu (%.3f, %.3f)x(%.3f, %.3f) to put planet %lu @ %.3fx%.3f\n", 
  //  startNodeId, node._ll[0], node._ll[1], node._ur[0], node._ur[1],
  //  planet.id(), planet.coordinate()[0], planet.coordinate()[1]);
  size_t pId = node.divide(tree);
  const Universe* universe = tree->universe();
  const Planet& occupyingPlanet = universe->planet(pId);
  size_t childNodeId = node.childNode(occupyingPlanet);
  TreeNode& childNode = tree->node(childNodeId);
  childNode.updateData(occupyingPlanet);
  childNode.putPlanet(occupyingPlanet);

  size_t newPlanetChildNodeId = node.childNode(planet);
  //printf("Node (%.3f, %.3f)x(%.3f, %.3f): occupyingPlanet %.3fx%.3f, incomingPlanet %.3fx%.3f\n", 
  //  node._ll[0], node._ll[1], node._ur[0], node._ur[1], 
  //  occupyingPlanet.coordinate()[0], occupyingPlanet.coordinate()[1], 
  //  planet.coordinate()[0], planet.coordinate()[1]);
  if (newPlanetChildNodeId == childNodeId) {
    //printf("Two planets occupying same node\n");
    return childNodeId;
  } else {
    TreeNode& newPlanetChildNode = tree->node(newPlanetChildNodeId);
    //printf("Incoming planet goes into node (%.3f, %.3f)x(%.3f, %.3f)\n", 
    //  newPlanetChildNode._ll[0], newPlanetChildNode._ll[1], 
    //  newPlanetChildNode._ur[0], newPlanetChildNode._ur[1]);
    newPlanetChildNode.updateData(planet);
    newPlanetChildNode.putPlanet(planet);
    return static_cast<size_t>(-1);
  }
  return static_cast<size_t>(-1);
}

static inline void
recursiveDivideAndAdd(const Planet& planet, 
             size_t startNodeId, BarnesHutTree* tree)
{
  size_t nodeId = startNodeId;
  while (nodeId != static_cast<size_t>(-1)) {
    nodeId = divideAndAdd(planet, nodeId, tree);
  }
}

void 
BarnesHutTree::addPlanet(const Planet& planet)
{
  const std::vector<size_t>& nodeIds = findTargetNodes(planet, this);
  size_t nodeSize = nodeIds.size();
  for (size_t i=0; i<nodeSize; ++i) {
    size_t nodeId = nodeIds[i];
    TreeNode& node = _treeNodes[nodeId];
    if (i == nodeIds.size() - 1 && node.occupied()) {
      recursiveDivideAndAdd(planet, nodeId, this);
    } else {
      node.updateData(planet);
      if (node.occupied() == false) {
        node.putPlanet(planet);
      }
    }
  }
}



}
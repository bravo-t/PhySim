#ifndef __NBD_BHALGO_H__
#define __NBD_BHALGO_H__

#include <deque>
#include <vector>
#include <cmath>

namespace NBody {

class Planet;
class Universe;
class BarnesHutTree;

struct TreeNode {
  TreeNode(size_t id, double urX, double urY, double llX, double llY) 
  : _nodeId(id) 
  {
    _ur[0] = urX;
    _ur[1] = urY;
    _ll[0] = llX;
    _ll[1] = llY;
  }

  double width() const { return fabs(_ur[0] - _ll[0]); }
  bool contains(const Planet& planet) const;
  size_t childNode(const Planet& planet) const;
  void updateData(const Planet& planet);
  size_t divide(BarnesHutTree* tree);
  bool occupied() const { return _planetId != static_cast<size_t>(-1); }
  void putPlanet(const Planet& planet);
  bool empty() const;
    
  size_t _nodeId = static_cast<size_t>(-1);
  double _ur[2];
  double _ll[2];
  double _totalMass = .0f;
  double _centerOfMass[2] = {.0f};
  size_t _planetId = static_cast<size_t>(-1);
  size_t _ulNode = static_cast<size_t>(-1);
  size_t _urNode = static_cast<size_t>(-1);
  size_t _llNode = static_cast<size_t>(-1);
  size_t _lrNode = static_cast<size_t>(-1);
};

class BarnesHutTree {
  public:
    BarnesHutTree(const std::deque<Planet>& planets, 
                  const Universe* universe);

    TreeNode& node(size_t id) { return _treeNodes[id]; }
    const TreeNode& node(size_t id) const { return _treeNodes[id]; }

    const Universe* universe() const { return _universe; }
    std::vector<const TreeNode*> childNodes(const TreeNode* node) const;

  private: 
    void addPlanet(const Planet& planet);
    std::vector<TreeNode> findNodes(const Planet& planet);


  private:
    const Universe* _universe;
    std::deque<TreeNode> _treeNodes;

  friend struct TreeNode;
};

}
#endif
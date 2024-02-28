#ifndef _NBD_PLANET_H_
#define _NBD_PLANET_H_

#include <string>

namespace NBody {

class Universe;

class Planet {
  public:
    Planet(size_t id) 
    : _id(id) {}
    Planet(const Planet& p) = default;

    void setCoordinate(double x, double y, double z = .0)
    {
      _coordinate[0] = x;
      _coordinate[1] = y;
      _coordinate[2] = z;
    }
    void setVelocity(double x, double y, double z = .0)
    {
      _velocity[0] = x;
      _velocity[1] = y;
      _velocity[2] = z;
    }
    void setMass(double m) { _mass = m; }
    std::string name(const Universe& universe) const;
    const double* coordinate() const { return _coordinate; }
    const double* velocity() const { return _velocity; }
    double mass() const {return _mass;}
    size_t id() const { return _id; }
  
  private:
    size_t _id = static_cast<size_t>(-1);
    double _coordinate[3] = {.0};
    double _velocity[3] = {.0};
    double _mass = .0;
};


}

#endif
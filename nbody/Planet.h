#ifndef _NBD_PLANET_H_
#define _NBD_PLANET_H_

#include <string>

namespace NBody {

class Planet {
  public:
    Planet(const std::string& name) 
    : _name(name) {}

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
    std::string name() const { return _name; }
    const double* coordinate() const { return _coordinate; }
    const double* velocity() const { return _velocity; }
    double mass() const {return _mass;}
  
  private:
    std::string _name;
    double _coordinate[3] = {.0};
    double _velocity[3] = {.0};
    double _mass = .0;
};


}

#endif
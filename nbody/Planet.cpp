#include "Planet.h"
#include "Universe.h"

namespace NBody {

std::string
Planet::name(const Universe& universe) const
{
  return universe.planetName(_id);
}

}
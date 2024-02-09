#ifndef _NBD_SIMULATOR_H_
#define _NBD_SIMULATOR_H_

namespace NBody {

enum SimulationMethod {
  PlainOldVanilla,

};

class Simulator {
  public:
    Simulator(SimulationMethod method = SimulationMethod::PlainOldVanilla);
    void run();

};

}

#endif
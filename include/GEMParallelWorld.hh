#ifndef GEMPARALLELWORLD_H
#define GEMPARALLELWORLD_H

#include "globals.hh"
#include "G4VUserParallelWorld.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

class GEMParallelWorld : public G4VUserParallelWorld
{
public:
    GEMParallelWorld(G4String worldName);
    ~GEMParallelWorld();

    void Construct();
    void ConstructSD();

private:
    G4LogicalVolume *GhostDetectorLog;
    G4LogicalVolume *GhostXBoxLog;
    G4LogicalVolume *GhostYBoxLog;
};
#endif


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
    virtual ~GEMParallelWorld();

    virtual void Construct();
    virtual void ConstructSD();

private:
    G4LogicalVolume *GhostTubeLog;
    G4LogicalVolume *GhostZTubeLog;

    G4VSensitiveDetector* sDetector;
};
#endif


#ifndef GEMVOXPARALLELWORLD_HH
#define GEMVOXPARALLELWORLD_HH

#include "globals.hh"
#include "G4VUserParallelWorld.hh"
#include "CLHEP/Units/SystemOfUnits.h"

using namespace CLHEP;

class GEMVoxParallelWorld : public G4VUserParallelWorld
{
public:
    GEMVoxParallelWorld(G4String worldName);
    virtual ~GEMVoxParallelWorld()
    {
        if (sDetector != NULL)
            delete sDetector;
    }

    virtual void Construct();
    virtual void ConstructSD();

private:
    G4LogicalVolume *GhostYBoxLog;
    G4VSensitiveDetector* sDetector;
};

#endif // GEMVOXPARALLELWORLD_HH

#ifndef GEMVOXPARALLELWORLD_HH
#define GEMVOXPARALLELWORLD_HH

#include "GEMDetectorSD.hh"

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
        if (ZeroDetector != NULL)
            delete ZeroDetector;

        if (PeakDetector != NULL)
            delete PeakDetector;
    }

    virtual void Construct();
    virtual void ConstructSD();

private:
    G4LogicalVolume *GhostYBoxZeroLog, *GhostYBoxPeakLog, *GhostDetectorZeroLog, *GhostDetectorPeakLog;
    G4VSensitiveDetector *ZeroDetector, *PeakDetector;
};

#endif // GEMVOXPARALLELWORLD_HH

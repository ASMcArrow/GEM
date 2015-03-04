#include "DebugTrajectoryPoint.hh"

G4ThreadLocal G4Allocator<DebugTrajectoryPoint> *DebugTrajectoryPointAllocator = 0;

DebugTrajectoryPoint::DebugTrajectoryPoint(G4ThreeVector pos, G4double energy, G4double edep)
{
    Position = pos;
    Energy = energy;
    Edep = edep;
}

DebugTrajectoryPoint::DebugTrajectoryPoint(const DebugTrajectoryPoint &right)
    : G4TrajectoryPoint(), Position(right.Position), Energy(right.Energy), Edep(right.Edep)
{}


#ifndef DEBUGTRAJECTORYPOINT_HH
#define DEBUGTRAJECTORYPOINT_HH

#include "G4TrajectoryPoint.hh"
#include "G4Trajectory.hh"
#include "G4AttDef.hh"

class DebugTrajectoryPoint : public G4TrajectoryPoint
{

public:
    DebugTrajectoryPoint(G4ThreeVector pos, G4double energy, G4double edep);
    DebugTrajectoryPoint(const DebugTrajectoryPoint &right);
    virtual ~DebugTrajectoryPoint() {}

    inline void *operator new(size_t);
    inline void operator delete(void *aTrajectoryPoint);
    inline int operator==(const DebugTrajectoryPoint& right) const
    { return (this == &right); }

    inline const G4ThreeVector GetPosition() const
    { return Position; }

    inline const G4double GetEnergy() const
    { return Energy; }

    inline const G4double GetEdep() const
    { return Edep; }


private:
    G4ThreeVector Position;
    G4double Energy;
    G4double Edep;
};

extern G4ThreadLocal G4Allocator<DebugTrajectoryPoint> *DebugTrajectoryPointAllocator;

inline void* DebugTrajectoryPoint::operator new(size_t)
{
    if (!DebugTrajectoryPointAllocator)
    {
        DebugTrajectoryPointAllocator = new G4Allocator<DebugTrajectoryPoint>;
    }
    return (void *) DebugTrajectoryPointAllocator->MallocSingle();
}

inline void DebugTrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
    DebugTrajectoryPointAllocator->FreeSingle((DebugTrajectoryPoint *) aTrajectoryPoint);
}

#endif // DEBUGTRAJECTORYPOINT_HH

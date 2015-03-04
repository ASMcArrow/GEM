#ifndef DEBUGTRAJECTORY_HH
#define DEBUGTRAJECTORY_HH

#include "G4Trajectory.hh"

class DebugTrajectory : public G4Trajectory
{

public:
    DebugTrajectory();
    DebugTrajectory(const G4Track* aTrack);
    DebugTrajectory(DebugTrajectory &right);
    virtual ~DebugTrajectory();

    inline void* operator new(size_t);
    inline void  operator delete(void*);
    inline int operator == (const DebugTrajectory& right) const
    { return (this == &right); }

    inline G4int GetParentID() const
    { return ParentID; }
    inline G4String GetParticleName() const
    { return ParticleName; }
    inline G4double GetCharge() const
    { return PDGCharge; }
    inline G4double GetInitialKineticEnergy() const
    { return InitialKineticEnergy; }
    inline G4ThreeVector GetInitialMomentum() const
    { return InitialMomentum; }
    int GetPointEntries() const
    { return PositionRecord->size(); }
    G4VTrajectoryPoint* GetPoint(G4int i) const
    { return (*PositionRecord)[i]; }

    // Other member functions
    virtual void ShowTrajectory(std::ostream& os = G4cout) const;
    virtual void DrawTrajectory() const;
    void MergeTrajectory(G4VTrajectory* secondTrajectory);

    virtual void AppendStep(const G4Step* aStep);

private:

    TrajectoryPointContainer* PositionRecord;
    G4int                     ParentID;
    G4double                  PDGCharge;
    G4String                  ParticleName;
    G4double                  InitialKineticEnergy;
    G4ThreeVector             InitialMomentum;
};

extern G4ThreadLocal G4Allocator<DebugTrajectory> *DebugTrajectoryAllocator;

inline void* DebugTrajectory::operator new(size_t)
{
    if (!DebugTrajectoryAllocator)
    {
        DebugTrajectoryAllocator = new G4Allocator<DebugTrajectory>;
    }
    return (void*)DebugTrajectoryAllocator->MallocSingle();
}

inline void DebugTrajectory::operator delete(void* aTrajectory)
{
    DebugTrajectoryAllocator->FreeSingle((DebugTrajectory*)aTrajectory);
}

#endif // DEBUGTRAJECTORY_HH

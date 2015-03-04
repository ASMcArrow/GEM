#ifndef DEBUGTRACKINGACTION_HH
#define DEBUGTRACKINGACTION_HH

#include "G4UserTrackingAction.hh"

class DebugTrackingAction : public G4UserTrackingAction
{

public:
    DebugTrackingAction() {}
    virtual ~DebugTrackingAction() {}

    void PreUserTrackingAction(const G4Track *aTrack);
    void ShutUp()
    {
        Verbosity = 0;
        SaveTrajectory = 0;
    }
    void SpillIt()
    {
        Verbosity = 1;
        SaveTrajectory = 0;
    }

private:
    int Verbosity = 0;
    bool SaveTrajectory = 0;
};

#endif // DEBUGTRACKINGACTION_HH
